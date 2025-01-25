#include <cassert>
#include <boost/di.hpp>

namespace di = boost::di;
struct renderer {
    int device;
};

class model {};

class user {};

class iview {
public:
    virtual ~iview() noexcept = default;
    virtual void update() = 0;
};

class gui_view : public iview {
public:
    gui_view(std::string /*title*/, const renderer& r) { assert(42 == r.device); }
    void update() override {}
};

class text_view : public iview {
public:
    void update() override {}
};

class controller {
public:
    controller(model&, iview&) {}
};

class app {
public:
    app(controller&, user&) {}
};

int main() {
    auto injector = di::make_injector(
            di::bind<iview>().to<gui_view>()
            , di::bind<int>().to(42) // renderer device
    );

    injector.create<app>();
}
