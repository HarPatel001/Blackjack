//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "../include/chat_message.hpp"
#include "../include/Card.hpp"
#include "../include/Deck.hpp"
#include "../include/Hand.hpp"



using asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------

class chat_participant
{
    public:
        virtual ~chat_participant() {}
        virtual void deliver(const chat_message& msg) = 0;
        int id = 0;
    private:
        //int id = 0;

};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room
{
    public:
        void join(chat_participant_ptr participant)
        {
            participants_.insert(participant);
            for (auto msg: recent_msgs_)
            {
                participant->deliver(msg);
                //participant->askBets();
            }
        }

        void leave(chat_participant_ptr participant)
        {
            participants_.erase(participant);
        }

        void deliver(const chat_message& msg)
        {
            recent_msgs_.push_back(msg);
            while (recent_msgs_.size() > max_recent_msgs)
                recent_msgs_.pop_front();

            for (auto participant: participants_)
                participant->deliver(msg);

        }

        // deliver msg to specific client
        void deliver2(const chat_message& msg, int recipient_id)
        {
            recent_msgs_.push_back(msg);
            while (recent_msgs_.size() > max_recent_msgs)
                recent_msgs_.pop_front();

            for (auto participant: participants_)
            {
                if(participant->id == recipient_id)
                    participant->deliver(msg);
            }
        }

        void askBets()
        {
            for (auto participant : participants_)
            {
                //participant->bets();
            }
        }





    private:
        std::set<chat_participant_ptr> participants_;
        enum { max_recent_msgs = 100 };
        chat_message_queue recent_msgs_;
};

//----------------------------------------------------------------------

class chat_session
: public chat_participant,
    public std::enable_shared_from_this<chat_session>
{
    public:
        chat_session(tcp::socket socket, chat_room& room)
            : socket_(std::move(socket)),
            room_(room)
    {

        //testing
        std::cout << "Dealer is here" << std::endl;

        shoe.build();
        shoe.shuffle();
        std::cout << "getting 2 cards" << std::endl;

        inHand.getCard();
        inHand.getCard();
        std::cout << "Hand is: " << inHand.getTotalValue() << std::endl;



    }

        void start()
        {
            room_.join(shared_from_this());
            do_read_header();
        }

        void deliver(const chat_message& msg)
        {
            bool write_in_progress = !write_msgs_.empty();
            write_msgs_.push_back(msg);
            if (!write_in_progress)
            {
                do_write();
            }
        }


        void bets()
        { 
            auto self(shared_from_this());
            asio::async_read(socket_,
                    asio::buffer(read_msg_.data(), chat_message::header_length),
                    [this, self](std::error_code ec, std::size_t /*length*/)
                    {
                    if (!ec       )
                    {
                    std::cout << "how much to bet?" << std::endl;
                    }

                    });
        }



    private:
        void do_read_header()
        {
            auto self(shared_from_this());
            asio::async_read(socket_,
                    asio::buffer(read_msg_.data(), chat_message::header_length),
                    [this, self](std::error_code ec, std::size_t /*length*/)
                    {
                    if (!ec && read_msg_.decode_header())
                    {
                    do_read_body();
                    }
                    else
                    {
                    room_.leave(shared_from_this());
                    }
                    });
        }

        void do_read_body()
        {
            auto self(shared_from_this());
            asio::async_read(socket_,
                    asio::buffer(read_msg_.body(), read_msg_.body_length()),
                    [this, self](std::error_code ec, std::size_t /*length*/)
                    {
                    if (!ec)
                    {
                    room_.deliver(read_msg_);
                    do_read_header();
                    }
                    else
                    {
                    room_.leave(shared_from_this());
                    }
                    });
        }

        void do_write()
        {
            auto self(shared_from_this());
            asio::async_write(socket_,
                    asio::buffer(write_msgs_.front().data(),
                        write_msgs_.front().length()),
                    [this, self](std::error_code ec, std::size_t /*length*/)
                    {
                    if (!ec)
                    {
                    write_msgs_.pop_front();
                    if (!write_msgs_.empty())
                    {
                    do_write();
                    }
                    }
                    else
                    {
                    room_.leave(shared_from_this());
                    }
                    });
        }


        //dealer to add card to hand
        void getCard()
        {
            Card temp;
            temp = shoe.getCard();
            temp.display();
            inHand.addToHand(temp);

        }




        tcp::socket socket_;
        chat_room& room_;
        chat_message read_msg_;
        chat_message_queue write_msgs_;

        Deck shoe;
        Hand inHand;
};

//----------------------------------------------------------------------

class chat_server
{
    public:
        chat_server(asio::io_context& io_context,
                const tcp::endpoint& endpoint)
            : acceptor_(io_context, endpoint)
        {
            do_accept();
        }

    private:
        void do_accept()
        {
            acceptor_.async_accept( // waiting for clients
                    [this](std::error_code ec, tcp::socket socket)
                    {
                    if (!ec)
                    {
                    std::make_shared<chat_session>(std::move(socket), room_)->start(); // start the chat_session
                    }

                    do_accept();
                    });
        }

        tcp::acceptor acceptor_;
        chat_room room_;
};

//----------------------------------------------------------------------

int main(int argc, char* argv[])
{ 
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }

        asio::io_context io_context;

        std::list<chat_server> servers;
        for (int i = 1; i < argc; ++i) // for if you type more than one port
        { 
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            servers.emplace_back(io_context, endpoint);
        }

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
