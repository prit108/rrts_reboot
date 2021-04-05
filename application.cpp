#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>

#include <gtkmm.h>

#include "road.hpp"
#include "area.hpp"
#include "city.hpp"
#include "complaint.hpp"
#include "user.hpp"
#include "clerk.hpp"
#include "supervisor.hpp"
#include "admin.hpp"

bool scheduling_done = false;
bool complaints_added = false;
User* currentUser;

using namespace std;
class RRTS : public Gtk::ApplicationWindow {
    public: 
    Glib::RefPtr<Gtk::Builder> _builder;
    Gtk::Stack* all_stack;
    Gtk::Button* login_btn, *add_compmenu_btn, *add_comp_btn, *clerk_logout_btn, *clerk_changepass_btn, *sup_logout_btn, *admin_logout_btn;
    Gtk::Button *sup_changepass_btn, *admin_changepass_btn, *change_cancel_btn;
    Gtk::ComboBoxText* clerk_road_dropdown;
    RRTS():_builder(Gtk::Builder::create_from_file("./ui_files/main_ui_structure.glade"))
    {
        if(_builder) {
            _builder->get_widget("all_stack",all_stack);
            //_builder->get_widget("clerk_mainmenu_grid", clerk_mainmenu_grid);
            Gtk::StackTransitionType ttype = Gtk::STACK_TRANSITION_TYPE_NONE;
            all_stack->set_visible_child("page0",ttype);

            add(*all_stack);
            _builder->get_widget("login_btn", login_btn);
            login_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::login_btn_clicked));
            
            _builder->get_widget("clerk_addcomp_btn", add_compmenu_btn);
            add_compmenu_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::addcompmenu_btn_clicked));

            _builder->get_widget("clerk_add_complaint_btn", add_comp_btn);
            add_comp_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::addcomp_btn_clicked));

            _builder->get_widget("clerk_logout_btn", clerk_logout_btn);
            clerk_logout_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::logout_btn_clicked));

            _builder->get_widget("sup_logout_btn", sup_logout_btn);
            sup_logout_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::logout_btn_clicked));

            _builder->get_widget("admin_logout_btn", admin_logout_btn);
            admin_logout_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::logout_btn_clicked));

            _builder->get_widget("clerk_road_dropdown", clerk_road_dropdown);
            for(Area c: City::sAreaList) {
                for(Road p : c.GetListRoads()) {
                    clerk_road_dropdown->append(p.ToString());
                }
            }

            _builder->get_widget("clerk_changepass_btn", clerk_changepass_btn);
            clerk_changepass_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::changepass_btn_clicked));

            _builder->get_widget("sup_changepass_btn", sup_changepass_btn);
            sup_changepass_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::changepass_btn_clicked));

            _builder->get_widget("admin_changepass_btn", admin_changepass_btn);
            admin_changepass_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::changepass_btn_clicked));

             _builder->get_widget("change_cancel_btn", change_cancel_btn);
            change_cancel_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::changecancel_btn_clicked));
        }
        set_title("RRTS");
        set_default_size(500,500);
        show_all();
    }
    void login_btn_clicked() {
        Gtk::StackTransitionType ttype = Gtk::STACK_TRANSITION_TYPE_NONE;
        Gtk::Entry *user,*passwordbox;
        _builder->get_widget("userid_textentry", user);
        // by design only numeric values are entered into the user entry field so no exception handling required
        _builder->get_widget("password_textentry", passwordbox);
        string userid = user->get_text();
        int user_id = atoi(userid.c_str());
        string password = passwordbox->get_text();
        string usertype;
        string username;
        if(User::IsValidLogin(user_id, password, usertype, username)) {
            if(usertype == "clerk") {
                all_stack->set_visible_child("page1", ttype);
                currentUser = new Clerk(username, user_id, password);
            }
            else if(usertype == "supervisor")
            {
                currentUser = new Supervisor(username, user_id, password);
                all_stack->set_visible_child("page7", ttype);
            } 
            else if(usertype == "admin") {
                *currentUser = Admin::CityAdmin(username, user_id, password);
                all_stack->set_visible_child("page8", ttype);
            }
            else {
                //Mayor functions
                all_stack->set_visible_child("page9", ttype);
            }
        }
        else{ 
            Gtk::Label *p;
            _builder->get_widget("login_msg_label", p);
            p->set_text("Invalid Login details.");
        }
        return;
    }

    void addcompmenu_btn_clicked () {
        if(complaints_added == false){
            Gtk::StackTransitionType ttype = Gtk::STACK_TRANSITION_TYPE_NONE;
            all_stack->set_visible_child("page2",ttype);
        }
        else {
            Gtk::Label* m;
            _builder->get_widget("clerk_add_comp_msg", m);
            m->set_text("Complaints have already been added for today. Thank you.");
        }
    }

    void addcomp_btn_clicked () {
        string selection = clerk_road_dropdown->get_active_text();
        cerr<<selection;
        if(selection.length() != 0) {
            string road = (string)selection;
            Gtk::Entry* matter;
            _builder->get_widget("clerk_complaint_matter_entry", matter);
            string mattertext = matter->get_text();
            if(Clerk::AddComplaintToDB(Complaint(City::Mumbai().GetRoadObject(road), mattertext))) {
                Gtk::Label* m;
                _builder->get_widget("clerk_add_err_msg_label", m);
                m->set_text("Successfully added complaint.");
            }
            else {
                Gtk::Label* m;
                _builder->get_widget("clerk_add_err_msg_label", m);
                m->set_text("Complaint addition failed.");
            }
        }
        else {
            Gtk::Label* m;
            _builder->get_widget("clerk_add_err_msg_label", m);
            m->set_text("Please select a road for the complaint.");
        }
    }

    void logout_btn_clicked() {
        Gtk::StackTransitionType ttype = Gtk::STACK_TRANSITION_TYPE_NONE;
        all_stack->set_visible_child("page0",ttype);
        Gtk::Entry *user,*passwordbox;
        _builder->get_widget("userid_textentry", user);
        _builder->get_widget("password_textentry", passwordbox);
        user->set_text("");
        passwordbox->set_text("");
        Gtk::Label *p;
        _builder->get_widget("login_msg_label", p);
        p->set_text("");
    }

    void changepass_btn_clicked() {
        Gtk::StackTransitionType ttype = Gtk::STACK_TRANSITION_TYPE_NONE;
        all_stack->set_visible_child("page3",ttype);
        Gtk::Entry *user,*oldpassbox, *newpassbox;
        _builder->get_widget("change_pass_user_entry", user);
        // by design only numeric values are entered into the user entry field so no exception handling required
        _builder->get_widget("change_old_pass_entry", oldpassbox);
        _builder->get_widget("change_new_pass_entry", newpassbox);
        string userid = user->get_text();
        int user_id = atoi(userid.c_str());
        string oldpassword = oldpassbox->get_text();
        string newpassword = newpassbox->get_text();
        string usertype, username;
        Gtk::Label *p;
        _builder->get_widget("change_pass_msg_label", p);
        if(User::IsValidLogin(user_id, oldpassword, usertype, username)) {
            if(User::UpdatePassword(user_id, newpassword)) {
                p->set_text("Password Changed Successfully");
                logout_btn_clicked();
            }
            else {
                p->set_text("Password Change Unsuccessfull.");
            }
        }
        else{
            p->set_text("Invalid credentials.");
        }
    }

    void changecancel_btn_clicked() {
        Gtk::StackTransitionType ttype = Gtk::STACK_TRANSITION_TYPE_NONE;
        if(currentUser->GetUserType() == "Clerk") {
            all_stack->set_visible_child("page1",ttype);
        } 
        else if(currentUser->GetUserType() == "Supervisor") {
            all_stack->set_visible_child("page7",ttype);
        }
        else if(currentUser->GetUserType() == "Admin") {
            all_stack->set_visible_child("page8",ttype);
        }
        else if(currentUser->GetUserType() == "Mayor") {
            all_stack->set_visible_child("page9",ttype);
        }
    }
};



int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(
        argc, argv, 
        "org.gtkmm.example.HelloApp"
    );
    RRTS hw;
    return app->run(hw);
}