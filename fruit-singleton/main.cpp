
#include <fruit/fruit.h>
#include <iostream>

using fruit::Component;
using fruit::Injector;

class Greeter {
public:
  virtual void greet() = 0;
};

class GreeterImpl : public Greeter {
public:
  // Like "GreeterImpl(Writer* writer) {...}" but also marks this constructor
  // as the one to use for injection.
    INJECT(GreeterImpl()) {
        std::cout << "GreeterImpl(" << this << ") is constructed." << std::endl;
  }

    ~GreeterImpl() {
        std::cout << "GreeterImpl(" << this << ") is destructed." << std::endl;
    }

  virtual void greet() override {
      std::cout << "Hello world from " << this << std::endl;
  }
};

Component<Greeter> getGreeterComponent() {
    return fruit::createComponent().bind<Greeter, GreeterImpl>();
}

Component<Greeter> getGreeterComponentSingleton() {
    static auto greeter = std::make_shared<GreeterImpl>();
    return fruit::createComponent().bind<Greeter, GreeterImpl>().bindInstance(*greeter);
}

int main() {

    {
        Injector<Greeter> injector(getGreeterComponent);
        std::shared_ptr<Greeter> greeter = injector.get<std::shared_ptr<Greeter>>();
        greeter->greet();
    }

    {
        Injector<Greeter> injector(getGreeterComponentSingleton);
        std::shared_ptr<Greeter> greeter = injector.get<std::shared_ptr<Greeter>>();
        greeter->greet();
    }

    {
        Injector<Greeter> injector(getGreeterComponentSingleton);
        std::shared_ptr<Greeter> greeter1 = injector.get<std::shared_ptr<Greeter>>();
        greeter1->greet();
        std::shared_ptr<Greeter> greeter2 = injector.get<std::shared_ptr<Greeter>>();
        greeter2->greet();
    }

    return 0;
}
