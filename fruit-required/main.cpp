#include <fruit/fruit.h>
#include <iostream>

using fruit::Component;
using fruit::Injector;
using fruit::Required;

class Writer {
public:
    virtual void write(const std::string& message) = 0;
};

class ConsoleWriter : public Writer {
public:
    INJECT(ConsoleWriter()) {}
    void write(const std::string& message) override {
        std::cout << message << std::endl;
    }
};

Component<Writer> getWriterComponent() {
    return fruit::createComponent()
            .bind<Writer, ConsoleWriter>();
}

class Greeter {
public:
    virtual void greet(const std::string& message) = 0;
};

class GreeterImpl : public Greeter {
private:
    Writer* writer;

public:
    explicit GreeterImpl(Writer* writer)
            : writer(writer) {
    }

    void greet(const std::string& message) override {
        writer->write(message);
    }
};

Component<Required<Writer>, Greeter> getGreeterComponent() {
    return fruit::createComponent()
            .registerConstructor<GreeterImpl(Writer*)>()
            .bind<Greeter, GreeterImpl>();
}

Component<Greeter> getGreeterImplComponent() {
    return fruit::createComponent().install(getWriterComponent).install(getGreeterComponent);
}
int main() {
    // Create an injector that provides both Writer and Greeter components
    Injector<Greeter> injector(getGreeterImplComponent);

    // Get a Greeter instance
    Greeter* greeter = injector.get<Greeter*>();

    // Use the Greeter instance (this will call the underlying Writer)
    greeter->greet("Hello, Google Fruit!");

    return 0;
}