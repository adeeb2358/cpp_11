#include "precompiled.h"

using namespace bandit;
using namespace std;

#define COMPILE_ERROR 0  // set to 1 to include invalid code in the compilation

class JetPlane
{
    string _model;
public:
    JetPlane() = default;
    JetPlane(const string& model) : _model(model) 
    {}

    string model() const { return _model; }

    void set_model(const string& model);
    void set_model(string&& model);
};

void JetPlane::set_model(const string& model)
{
    _model = model;
}

void JetPlane::set_model(string&& model)
{
    _model = move(model); // careful: model is a named rvalue ref so it's an
                          // lvalue; use std::move to force a move operation
    model.clear();
}


// perfect forwarding can be used to implement make_unique
template<typename T, typename... Args> 
unique_ptr<T> make_unique(Args&&... args) 
{
    return unique_ptr<T>(new T(forward<Args>(args)...));
}

go_bandit([] {

    describe("language support for perfect forwarding:", [] {

        it("allows perfect forwarding", [&] {
            struct Point
            {
                int _x, _y;
                Point(int x, int y) : _x(x), _y(y)
                {}
                Point() = default;
                Point(const Point&) = default;
            };
            unique_ptr<vector<Point>> p_points(new vector<Point>(10));

            auto p = make_unique<Point>(10, 20);

            // example of double reference
            Point p1(10, 10);
            using PointRef = Point&;
            PointRef& p2 = p1;

            // example of using make_unique
            string model("Boeing 787");
            auto sp = make_unique<JetPlane>(model);
            AssertThat(sp->model(), Equals(model));
        });

    });

});

int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}

