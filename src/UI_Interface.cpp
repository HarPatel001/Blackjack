#include "../include/UI_Interface.h"
//#include "../include/client.h"
#include <gtkmm.h>
#include <string>

UI_Interface::UI_Interface()
{
    set_default_size(700, 700);
    set_title("BJ");

    vbox = Gtk::manage(new Gtk::VBox);
    add(*vbox);

    menubar = Gtk::manage(new Gtk::MenuBar());
    vbox->pack_start(*menubar, Gtk::PackOptions::PACK_SHRINK, 0); //pack_shrink packs it shrinked, 0 for padding

    Gtk::MenuItem *menuitem_file = Gtk::manage(new Gtk::MenuItem("_File", true));
    menubar->append(*menuitem_file);

    Gtk::Menu *filemenu = Gtk::manage(new Gtk::Menu());
    menuitem_file->set_submenu(*filemenu);

    Gtk::MenuItem *menuitem_new = Gtk::manage(new Gtk::MenuItem("_New Game", true));
    menuitem_new->signal_activate().connect([this] { this->on_new_clicked(); });
    filemenu->append(*menuitem_new);

    grid = Gtk::manage(new Gtk::Grid);
    grid->set_hexpand(true);
    grid->set_vexpand(true); 

    // reverse for j because grid is added top down

    std::string tile_image = "images/cards/2s.jpg";

    Gtk::Button *hit_button = Gtk::manage(new Gtk::Button("HIT"));
    hit_button->signal_clicked().connect([this] { this->hit_button_pressed(); });
    Gtk::Button *stand_button = Gtk::manage(new Gtk::Button("STAND"));
    Gtk::Button *double_button = Gtk::manage(new Gtk::Button("DOUBLE"));
    Gtk::Button *split_button = Gtk::manage(new Gtk::Button("SPLIT"));
    Gtk::Button *leave_button = Gtk::manage(new Gtk::Button("LEAVE"));

    Gtk::Label *d_info = Gtk::manage(new Gtk::Label());
    d_info->set_label("Dealer :");

    int i;
    std::string l;

    Gtk::Image *d_first_card = Gtk::manage(new Gtk::Image(tile_image));
    Gtk::Image *d_second_card = Gtk::manage(new Gtk::Image(tile_image));

    Gtk::Image *d_sp_first_card = Gtk::manage(new Gtk::Image(tile_image));
    Gtk::Image *d_sp_second_card = Gtk::manage(new Gtk::Image(tile_image));

    //-----------------------------------------------------------------------------------------

    Gtk::Image *p1_first_card = Gtk::manage(new Gtk::Image(tile_image));
    Gtk::Image *p1_second_card = Gtk::manage(new Gtk::Image(tile_image));

    Gtk::Image *p1_sp_first_card = Gtk::manage(new Gtk::Image(tile_image));
    Gtk::Image *p1_sp_second_card = Gtk::manage(new Gtk::Image(tile_image));

    //-----------------------------------------------------------------------------------------

    Gtk::Image *p2_first_card = Gtk::manage(new Gtk::Image(tile_image));
    Gtk::Image *p2_second_card = Gtk::manage(new Gtk::Image(tile_image));

    Gtk::Image *p2_sp_first_card = Gtk::manage(new Gtk::Image(tile_image));
    Gtk::Image *p2_sp_second_card = Gtk::manage(new Gtk::Image(tile_image));

    //-----------------------------------------------------------------------------------------

    Gtk::Image *p3_first_card = Gtk::manage(new Gtk::Image(tile_image));
    Gtk::Image *p3_second_card = Gtk::manage(new Gtk::Image(tile_image));

    Gtk::Image *p3_sp_first_card = Gtk::manage(new Gtk::Image(tile_image));
    Gtk::Image *p3_sp_second_card = Gtk::manage(new Gtk::Image(tile_image));

    //-----------------------------------------------------------------------------------------

    Gtk::Image *p4_first_card = Gtk::manage(new Gtk::Image(tile_image));
    Gtk::Image *p4_second_card = Gtk::manage(new Gtk::Image(tile_image));

    Gtk::Image *p4_sp_first_card = Gtk::manage(new Gtk::Image(tile_image));
    Gtk::Image *p4_sp_second_card = Gtk::manage(new Gtk::Image(tile_image));

    //-----------------------------------------------------------------------------------------

    Gtk::Image *p5_first_card = Gtk::manage(new Gtk::Image(tile_image));
    Gtk::Image *p5_second_card = Gtk::manage(new Gtk::Image(tile_image));

    Gtk::Image *p5_sp_first_card = Gtk::manage(new Gtk::Image(tile_image));
    Gtk::Image *p5_sp_second_card = Gtk::manage(new Gtk::Image(tile_image));

    //-----------------------------------------------------------------------------------------

    Gtk::Image *p6_first_card = Gtk::manage(new Gtk::Image(tile_image));
    Gtk::Image *p6_second_card = Gtk::manage(new Gtk::Image(tile_image));

    Gtk::Image *p6_sp_first_card = Gtk::manage(new Gtk::Image(tile_image));
    Gtk::Image *p6_sp_second_card = Gtk::manage(new Gtk::Image(tile_image));

    //-----------------------------------------------------------------------------------------

    grid->attach(*d_info,1,1,1,1);
    grid->attach(*d_first_card,2,1,1,1);
    grid->attach(*d_second_card,3,1,1,1);    



    for(i=1;i<6;i++){
        label = Gtk::manage(new Gtk::Label());
        l = std::to_string(i);
        label->set_label("player" + l);
        grid->attach(*label,i,2,1,1);
    }

    grid->attach(*hit_button,6,3,1,1);

    grid->attach(*p1_first_card,1,3,1,1);
    grid->attach(*p2_first_card,2,3,1,1);
    grid->attach(*p3_first_card,3,3,1,1);
    grid->attach(*p4_first_card,4,3,1,1);
    grid->attach(*p5_first_card,5,3,1,1);

    grid->attach(*stand_button,7,3,1,1);

    grid->attach(*p1_second_card,1,3,1,1);
    grid->attach(*p2_second_card,2,3,1,1);
    grid->attach(*p3_second_card,3,3,1,1);
    grid->attach(*p4_second_card,4,3,1,1);
    grid->attach(*p5_second_card,5,3,1,1);            

    grid->attach(*split_button,7,4,1,1);
    grid->attach(*leave_button,8,3,1,1);

    grid->set_row_spacing(10);
    grid->set_column_spacing(10); 

    vbox->add(*grid);
    vbox->add(*p2_sp_first_card);
    vbox->add(*p3_sp_first_card);

    vbox->show_all();

    split_button->show();
}

UI_Interface::~UI_Interface() {}

void UI_Interface::on_button_clicked()
{

}

void UI_Interface::on_new_clicked()
{

}

void UI_Interface::set_id(int id)
{
    UI_Interface::id = id;
}

void UI_Interface::hit_button_pressed()
{
    //Connector::hit(UI_Interface::id);
}
