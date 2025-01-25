#include <string>
#include <boost/di.hpp>

namespace di = boost::di;

struct renderer {
    int device;
};

class view {
public:
    view(std::string /*title*/, const renderer&) {}
};

class model {};

class controller {
public:
    controller(model&, view&) {}
};

class user {};

class app {
public:
    app(controller&, user&) {}
};

int main() {
    /**
     * renderer renderer_;
     * view view_{"", renderer_};
     * model model_;
     * controller controller_{model_, view_};
     * user user_;
     * app app_{controller_, user_};
     */

    auto injector = di::make_injector();
    injector.create<app>();
}