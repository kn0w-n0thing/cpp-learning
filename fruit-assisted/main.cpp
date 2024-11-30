#include <fruit/fruit.h>
#include <iostream>

using fruit::Component;
using fruit::Injector;
using fruit::Required;
using fruit::createComponent;

class Writer {
public:
    explicit Writer(const std::string& message) {};
    virtual ~Writer() = default;
    virtual void write() = 0;
};

class WriterImpl : public Writer {
public:
    INJECT(WriterImpl(ASSISTED(std::string) message)): Writer(message) {
        message_ = std::make_unique<std::string>(message);
    }
    void write() override {
        std::cout << *message_ << std::endl;
    }

private:
    std::unique_ptr<std::string> message_;
};

using UniqueWriterFactory = std::function<std::unique_ptr<Writer>(std::string)>;

Component<UniqueWriterFactory> getUniqueWriterFactory() {
    return createComponent().bind<Writer, WriterImpl>();
}

fruit::Component<std::string> getMessageComponent() {
    return fruit::createComponent()
            .registerProvider([]() {
                return std::string("Required string injection!");
            });
}

fruit::Component<Required<std::string>, Writer> getWriterRequiredString() {
    return fruit::createComponent()
            .registerConstructor<WriterImpl(std::string)>()
            .bind<Writer, WriterImpl>();

}

fruit::Component<Writer> getWriterComponent() {
    return fruit::createComponent()
            .install(getWriterRequiredString)
            .install(getMessageComponent);
}

int main() {
    // Create an injectorAssisted that provides both Writer and Greeter components
    Injector<UniqueWriterFactory> injectorAssisted(getUniqueWriterFactory);

    UniqueWriterFactory factory = injectorAssisted.get<UniqueWriterFactory>();

    std::unique_ptr<Writer> unique_writer = factory("Unique assisted injection!");
    unique_writer->write();

    // Use the factory to create a Writer
    std::shared_ptr<Writer> shared_writer1 = factory("Shared assisted injection!");
    auto shared_writer2 = shared_writer1;

    // Use the Greeter instance (this will call the underlying Writer)
    shared_writer1->write();
    shared_writer2->write();
    std::cout << "Count the shared writer is: " << shared_writer1.use_count() << std::endl;

    Injector<Writer> injectorRequired(getWriterComponent);
    std::shared_ptr<Writer> shared_writer3 = injectorRequired.get<std::shared_ptr<Writer>>();
    shared_writer3->write();

    return 0;
}