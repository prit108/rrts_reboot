#ifndef COMPLAINT_HPP
#define COMPLAINT_HPP
#include <string>
#include <tuple>
#include "road.hpp"

using namespace std;

class Complaint {
    int id_;
    Road road_;
    string matter_;
    tuple <int, int, double, int, int, int> resources_; //(cement bags, sand bags, water, workers, machines, slots)
    int priority_;
    bool isPending_;
public:
    Complaint(const int id, const Road& road, const string& matter, const bool isPending);

    Complaint(const Complaint& c):
    id_(c.id_), road_(c.road_), matter_(c.matter_), resources_(c.resources_), priority_(c.priority_), isPending_(c.isPending_) {}

    ~Complaint () {}

    const Road& GetRoad() {
        return this->road_;
    }

    bool IsPending() {
        return this->isPending_;
    }

    void SetPending(bool b) {
        this->isPending_ = b;
    }

    int GetId() {
        return this->id_;
    }

    tuple <int, int, double, int, int, int> GetResources() const {
        return this->resources_;
    }

    void SetResources(tuple <int, int, double, int, int, int> t) {
        this->resources_ = t;
    }

    int GetPriority() const {
        return this->priority_;
    }

    void SetPriority(int n) {
        this->priority_ = n;
    }

    const string& GetComplaintMatter() {
        return this->matter_;
    }

    friend ostream& operator<<(ostream& os, const Complaint& p) {
        os << "Complaint ID::"<<p.id_ << ", Concerned Road::" <<p.road_;
        return os;
    }
};

#endif
