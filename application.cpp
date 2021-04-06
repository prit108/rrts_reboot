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
bool assignment_done = false;
bool complaints_added = false;
User* currentUser;
vector<Complaint> fresh_complaints;
Supervisor* currSup;
vector<Complaint> alltobedone;
vector<pair<Complaint, int>> todayschedule;
vector<Complaint> pended_today;
vector<Complaint> completed_today;

vector<Supervisor> allSup;
int count_done = 0;

void print_schedule () {
    cout<<"The Schedule : "<<endl;
    for(auto x : todayschedule) {
        cout<< x.first.ToString() << " :: Slot : "<< x.second << endl; 
    }
}

bool CheckPriority(const vector<Complaint>& comp) {
    cerr<<"IN Check_Priority"<<endl;
    int i, n;
    n=comp.size();
    vector<int> assigned(n,0);
    for(i=0;i<n;i++){
        if(comp[i].GetPriority()<=0 || comp[i].GetPriority()>n)
            return false;
        else assigned[comp[i].GetPriority()-1]++;
    }
    for(i=0;i<n;i++){
        if(assigned[i]!=1)
            return false;
    }
    return true;
}

string GetPrintableEntry(Complaint x) {
    string out = "ComplaintId#";
    out = out + to_string(x.GetId()) + ",   " + x.GetRoad().ToString() + ",   Area " + City::Mumbai().GetArea(x.GetRoad()).GetName() + ",    Supervisor :" + Supervisor::GetAreaSupervisor(allSup, City::Mumbai().GetArea(x.GetRoad())).GetName() + ",    Resources :" + to_string(get<0>(x.GetResources()));
    return out;
}


using namespace std;
class RRTS : public Gtk::ApplicationWindow {
    public: 
    Glib::RefPtr<Gtk::Builder> _builder;
    Gtk::Stack* all_stack;
    Gtk::Button* login_btn, *add_compmenu_btn, *add_comp_btn, *clerk_logout_btn, *clerk_changepass_btn, *sup_logout_btn, *admin_logout_btn;
    Gtk::Button *sup_changepass_btn, *admin_changepass_btn, *change_cancel_btn, *change_pass_btn;
    Gtk::Button *clerk_end_process_btn, *sup_assign_btn, *assign_priority_btn, *spvsr_end_process_btn;
    Gtk::Button *sup_get_schedule_btn;
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

            _builder->get_widget("clerk_back_to_menu_btn", clerk_end_process_btn);
            clerk_end_process_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::clerk_end_btn_clicked));


            _builder->get_widget("clerk_changepass_btn", clerk_changepass_btn);
            clerk_changepass_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::passreq_btn_clicked));

            _builder->get_widget("sup_changepass_btn", sup_changepass_btn);
            sup_changepass_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::passreq_btn_clicked));

            _builder->get_widget("admin_changepass_btn", admin_changepass_btn);
            admin_changepass_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::passreq_btn_clicked));

             _builder->get_widget("change_cancel_btn", change_cancel_btn);
            change_cancel_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::changecancel_btn_clicked));

            _builder->get_widget("change_pass_btn", change_pass_btn);
            change_pass_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::changepass_btn_clicked));

            _builder->get_widget("sup_assign_btn", sup_assign_btn);
            sup_assign_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::supassign_btn_clicked));

            _builder->get_widget("assign_priority_btn", assign_priority_btn);
            assign_priority_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::assign_priority_btn_clicked));

            _builder->get_widget("spvsr_end_process_btn", spvsr_end_process_btn);
            spvsr_end_process_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::spvsr_end_process_btn_clicked));

            _builder->get_widget("show_schedule_btn", sup_get_schedule_btn);
            sup_get_schedule_btn->signal_clicked().connect(sigc::mem_fun(*this,&RRTS::sup_schedule_btn_clicked));
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
            Complaint p(City::Mumbai().GetRoadObject(road), mattertext);
            if(Clerk::AddComplaintToDB(p)) {
                Gtk::Label* m;
                _builder->get_widget("clerk_add_err_msg_label", m);
                m->set_text("Successfully added complaint.");
                fresh_complaints.push_back(p);
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


    void passreq_btn_clicked() {
        Gtk::StackTransitionType ttype = Gtk::STACK_TRANSITION_TYPE_NONE;
        all_stack->set_visible_child("page3",ttype);
    }

    void changepass_btn_clicked() {
        Gtk::Label *p;
        _builder->get_widget("change_pass_msg_label", p);
        p->set_text("");
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
            Gtk::Label *p;
            _builder->get_widget("sup_add_comp_msg", p);
            p->set_text("");
            all_stack->set_visible_child("page7",ttype);
        }
        else if(currentUser->GetUserType() == "Admin") {
            all_stack->set_visible_child("page8",ttype);
        }
        else if(currentUser->GetUserType() == "Mayor") {
            all_stack->set_visible_child("page10",ttype);
        }
    }
    void clerk_end_btn_clicked() {
        complaints_added = true;
        Gtk::StackTransitionType ttype = Gtk::STACK_TRANSITION_TYPE_NONE;
        all_stack->set_visible_child("page1",ttype);
    }
    void supassign_btn_clicked() {
        if(!complaints_added || assignment_done) {
            Gtk::Label *p;
            _builder->get_widget("sup_add_comp_msg", p);
            p->set_text("Complaints are yet to be added. Please wait. Contact office for more details.");
        }
        else {
            bool safe = true;
            cerr<<currentUser->GetID()<<endl;
            allSup.push_back(Supervisor(currentUser->GetName(), currentUser->GetID(), currentUser->GetPassword()));
            cerr<<allSup.size()<<endl;
                cerr<<allSup.at(allSup.size()-1).GetName()<<endl;
            if(!Supervisor::GetAssignedAreaList(allSup.at(allSup.size() - 1))) {
                cerr<<"ERROR::Database error, aborting."<<endl;
                safe = false;
            }
            Gtk::Label *p;
            _builder->get_widget("spvsr_priority_msg", p);
            p->set_text("");
            Gtk::StackTransitionType ttype = Gtk::STACK_TRANSITION_TYPE_NONE;
            //if(!safe){ return; }
            //else {
                allSup.at(allSup.size() - 1).SetComplaints(fresh_complaints);
                cerr<<allSup.size()<<endl;
                cerr<<allSup.at(allSup.size()-1).GetName()<<endl;
                Gtk::ComboBoxText* sup_assign_complaint_list, *sup_assign_priority_list; 
                _builder->get_widget("sup_assign_complaint_list", sup_assign_complaint_list);
                _builder->get_widget("sup_assign_priority_list", sup_assign_priority_list);
                int i = 1;
                for(Complaint c: allSup.at(allSup.size() - 1).GetAssignedComplaints()) {
                    sup_assign_complaint_list->append(c.ToString());
                    sup_assign_priority_list->append(to_string(i));
                    cerr<<c.ToString()<<endl;
                    i++;
                }
                
                all_stack->set_visible_child("page4",ttype);
                return;
            //}
        }
    }
    void assign_priority_btn_clicked () {
        Gtk::Label *p;
        _builder->get_widget("spvsr_priority_msg", p);
        p->set_text("");
        Gtk::ComboBoxText* sup_complaint_list, *sup_priority_list, *sup_slot_list;
        _builder->get_widget("sup_assign_priority_list", sup_priority_list);
        _builder->get_widget("sup_assign_complaint_list", sup_complaint_list);
        _builder->get_widget("slot_dropdown", sup_slot_list);
        Gtk::Entry* cement, *sand, *labor, *machine;
        _builder->get_widget("cement_bags_entry", cement);
        _builder->get_widget("sand_bags_entry", sand);
        _builder->get_widget("labor_entry", labor);
        _builder->get_widget("machine_entry", machine);
        sup_slot_list->set_active_text("1");
        //sup_priority_list->set_active_text("1");
        string complaint = sup_complaint_list->get_active_text();
        int priority = atoi(sup_priority_list->get_active_text().c_str());
        int slot = atoi(sup_slot_list->get_active_text().c_str());
        int cement_bags = cement->get_text() == "" ? -1 : atoi(cement->get_text().c_str());
        int sand_bags = sand->get_text() == "" ? -1 : atoi(sand->get_text().c_str());
        int labourers = labor->get_text() == "" ? -1 : atoi(labor->get_text().c_str());
        int machines = machine->get_text() == "" ? -1 : atoi(machine->get_text().c_str());
        if(cement_bags < 0 || sand_bags < 0 || labourers < 0 || machines < 0)
        {
            Gtk::Label *p;
            _builder->get_widget("spvsr_priority_msg", p);
            p->set_text("Invalid resource inputs.");
            return;
        }
        cerr<<complaint<<endl;
        allSup.at(allSup.size() - 1).SetResourcesThisComplaint(complaint, priority ,make_tuple(cement_bags, sand_bags, 0.0, labourers, machines, slot));
    }
    void spvsr_end_process_btn_clicked() {
        cerr<<"END_PROCESS_CLICKED"<<endl;
        Gtk::Label *p;
        _builder->get_widget("spvsr_priority_msg", p);
        p->set_text("");
        if(CheckPriority(allSup.at(allSup.size() - 1).GetAssignedComplaints()))
        {   
            cerr<<"Priority is Correct."<<endl;
            bool flag = true;
            for(Complaint x : allSup.at(allSup.size() - 1).GetAssignedComplaints()) {
                count_done++;
                if(!Supervisor::PushResourcesToDB(x)) {
                    cerr<<"Database Error"<<endl;
                    flag = false;
                }
            }
            if(flag) {
                Gtk::Label *p;
                _builder->get_widget("sup_add_comp_msg", p);
                p->set_text("");
                Gtk::StackTransitionType ttype = Gtk::STACK_TRANSITION_TYPE_NONE;
                all_stack->set_visible_child("page7",ttype);
                cerr<<"Flag is true."<<endl;
                if(count_done == fresh_complaints.size())
                {
                    cerr<<"Flag is true."<<endl;
                    assignment_done = true;
                    Admin::GetTodayComplaint(alltobedone);
                    todayschedule = Admin::Schedule(alltobedone);
                    print_schedule();
                    scheduling_done = true;
                }
            }
        }
        else {
            p->set_text("Invalid Priority Assignment. Not saved.");
        }  
    }
    void sup_schedule_btn_clicked () {
        if(!scheduling_done) {
            Gtk::Label *p;
            _builder->get_widget("sup_add_comp_msg", p);
            p->set_text("Scheduling not yet done. Please wait. Contact office for more details.");
            return;
        }
        else {
            completed_today.clear();
            pended_today.clear();
            string morning_schedule = "\n", 
            string evening_schedule = "\n";
            /*
            vector<Complaint> pended_today;
            vector<Complaint> completed_today;
            */
            int m = 0, e = 0;
            for(auto x : todayschedule) {
                if (x.second == 0) {
                    m++;
                    morning_schedule += to_string(m) + ". " + GetPrintableEntry(x.first) + "\n";
                    completed_today.push_back(x.first);
                }

                if (x.second == 1) {
                    e++;
                    evening_schedule += to_string(e) + ". " + GetPrintableEntry(x.first) + "\n";
                    completed_today.push_back(x.first);
                }

                if (x.second == -1) {
                    pended_today.push_back(x.first);
                }
            }
            Glib::RefPtr<Gtk::TextBuffer> b1,b2;
            b1->set_text(morning_schedule);
            b2->set_text(evening_schedule);
            Gtk::TextView *morn, *eve;
            _builder->get_widget("morning_textview", morn);
            _builder->get_widget("evening_textview", eve);
            morn->set_buffer(b1);
            eve->set_buffer(b2);
            Gtk::StackTransitionType ttype = Gtk::STACK_TRANSITION_TYPE_NONE;
            all_stack->set_visible_child("page5",ttype);
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