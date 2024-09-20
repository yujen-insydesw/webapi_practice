#include <iostream>

//#include <thread>
    // Simulate server running for some time
    //std::this_thread::sleep_for(std::chrono::seconds(3));

#include "../include/controllers/person_controller.hpp"
#include "../include/router.hpp"
#include "../include/server.hpp"
#include "../include/services/person_service.hpp"

int main(void) {
  try {
    auto router = std::make_shared<Router>();
    //auto server = ThreadDetachServer(6969, router);
    auto server = AsyncAcceptServer(6969, router);
    auto personService = std::make_shared<PersonService>();
    auto personController = std::make_shared<PersonController>(personService);

    router->setPrefix("/v1");

    router->addRoute(GET, "/person", [personController](auto &ctx) {
      personController->getPersons(ctx);
    });

    router->addRoute(POST, "/person", [personController](auto &ctx) {
      personController->createPerson(ctx);
    });

    router->addRoute(GET, "/person/{id}", [personController](auto &ctx) {
      personController->getPersonById(ctx);
    });

    router->addRoute(DELETE, "/person/{id}", [personController](auto &ctx) {
      personController->deletePersonById(ctx);
    });

    std::cout << "Server starting on port " << server.getPort() << std::endl;
    server.run();


  } catch (std::exception const &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}


/*


optional

Why Use std::optional in C++?
std::optional is a feature introduced in C++17 that represents an optional value, meaning a value that may or may not be present. Here are some reasons to use std::optional:

Expressing Optional Values: It clearly indicates that a value might be absent, making your code more readable and expressive.
Avoiding Sentinel Values: Instead of using special sentinel values (like -1 or nullptr) to indicate the absence of a value, std::optional provides a type-safe way to handle optional values.
Improved Error Handling: It simplifies error handling by avoiding the need for separate error codes or flags.
Better Code Documentation: It documents the intent of your code, making it clear when a function might not return a value12.
How to Use std::optional
Here’s a basic guide on how to use std::optional:

1. Including the Header
First, include the <optional> header:

#include <optional>

2. Creating an std::optional
You can create an std::optional object in several ways:

std::optional<int> opt1; // Empty optional
std::optional<int> opt2 = 42; // Optional with a value
auto opt3 = std::make_optional(42); // Using make_optional

3. Checking for a Value
Before accessing the value, check if it exists:

if (opt2.has_value()) {
    std::cout << "Value: " << opt2.value() << std::endl;
} else {
    std::cout << "No value" << std::endl;
}

4. Accessing the Value
You can access the value using value() or the dereference operator *:

int value = opt2.value(); // Throws if no value
int value = opt2.value_or(0); // Returns 0 if no value
int value = *opt2; // Dereference operator

5. Resetting the Value
You can reset an std::optional to make it empty:

opt2.reset();

Example Code
Here’s a complete example demonstrating the use of std::optional:

#include <iostream>
#include <optional>
#include <string>

std::optional<std::string> getName(bool returnName) {
    if (returnName) {
        return "John Doe";
    } else {
        return std::nullopt;
    }
}

int main() {
    auto name = getName(true);
    if (name) {
        std::cout << "Name: " << *name << std::endl;
    } else {
        std::cout << "No name provided" << std::endl;
    }

    name = getName(false);
    if (name) {
        std::cout << "Name: " << *name << std::endl;
    } else {
        std::cout << "No name provided" << std::endl;
    }

    return 0;
}



cast

https://stackoverflow.com/questions/28002/regular-cast-vs-static-cast-vs-dynamic-cast



////////////////////////////////////////////////

#include <iostream>
#include <functional>

void processNumbers(std::function<int(int)> callback) {
    int result = callback(8);
    std::cout << "Result: " << result << std::endl;
}


/*
Differences and Preferences
Readability: Many developers find using to be more readable and intuitive, especially for complex type definitions.
Template Aliases: using can be used to create template aliases, which typedef cannot do. For example:
template <typename T>
using Vec = std::vector<T>;

Consistency: using aligns with other modern C++ features and syntax, making it a preferred choice in contemporary C++ codebases.
*/
//typedef std::function<int(int)> Callback;

using Callback = std::function<int(int)>;

//void processNumbers(Callback callback) {...}


class TEST {
private:
    int hey = 33;
public:
    static int test(int a) { return 3; }

    int non_test(int a) const {
        //hey = -3; invalid
        return 3; }

    void lambda_test() {
        auto func = [this] (int a) {
            hey = 4;
            TEST::test(4);// use static memeber, don't need this ptr
            non_test(4);
        };
        func(4);
    }

};

int test(int a) { return 3; }

int main()
{
    int m = 3;
    const int &pm = m;
    int n = 6;
    const int *pn = &n;

// Define lambda
    auto lambdaFunc = [&pm, pn] (int a) {
        // *pn = 3; invalid
        // pm = 3; invalid
        return pm + *pn + a;
    };
    std::cout << f( << m << std::endl << func1(4) << n << std::endl << std::endl;
    
// Store callback
    //const Callback cb = lam... TEST::test; test;

// Pass callback
    processNumbers(lambdaFunc);

    return 0;
}



////////////////////////////////////////////////

/*
No, std::shared_ptr and std::unique_ptr cannot be directly cast to each other. They have different ownership semantics and are designed for different use cases:

std::unique_ptr
Exclusive Ownership

*/

#include <iostream>
#include <memory> // for modern pointer

class Object {
public:
    Object(int value) : data(value) {
        std::cout << "Object created with value: " << data << std::endl;
    }
    ~Object() {
        std::cout << "Object destroyed with value: " << data << std::endl;
    }
    int data;
};

int main() {

    //
    // Unique pointer
    //

    // Creating a unique_ptr
    std::unique_ptr<Object> uniqueObject = std::make_unique<Object>(3);

    // Transferring ownership to another unique_ptr using std::move
    std::unique_ptr<Object> uniqueObject2 = std::move(uniqueObject);
    if (!uniqueObject) {
        std::cout << "uniqueObject is null after move." << std::endl;
    }

    //
    // Share pointer & Weak pointer
    //

    // Creating a shared pointer with resource ownership
    std::shared_ptr<Object> sharedObjectA = std::make_shared<Object>(33);
    //
    // Sharing from existedd object
    //Object obj(333);
    //auto sharedObjectA = std::make_shared<Object>(std::move(obj)); // just transfer ownership, without deep copy
    //auto sharedObjectA = std::make_shared<Object>(obj); // with deep copy

    // Creating a weak pointer to the previously created shared object
    std::weak_ptr<Object> weakObjectA = sharedObjectA;

    // Accessing the object using weak_ptr
    if (auto lockedObject = weakObjectA.lock()) { // lock() returns a shared_ptr
        std::cout << "The value stored in sharedObjectA: " << lockedObject->data << std::endl;
    } else {
        std::cout << "The object has been destroyed." << std::endl;
    }

    // Resetting the shared pointer, which will destroy the object
    sharedObjectA.reset();

    // Checking the weak pointer again
    if (auto lockedObject = weakObjectA.lock()) {
        std::cout << "The value stored in sharedObjectA: " << lockedObject->data << std::endl;
    } else {
        std::cout << "The object has been destroyed." << std::endl;
    }

    return 0;
}




//////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <thread>
#include <functional> // callback type
#include <future>     // async

void onTaskComplete(int result) {
    std::cout << "Task completed with result: " << result << std::endl;
}

// A function that runs asynchronously and calls the callback
void asyncFunction(std::function<void(int)> callback, int x) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    callback(x);
}

int main() {
    int value = 10;

    //
    // std::async
    //

    // Run the async function with the callback
    std::future<void> result = std::async(std::launch::async,
        asyncFunction,
            onTaskComplete, value
        );

    // Do other work while the async function runs
    std::cout << "Doing other work in main thread..." << std::endl;

    // Wait for the async function to complete
    result.get(); // type ret = result.get(); if return value

    //
    // With lambda function
    //
    
    // Perform the task asynchronously
    std::async(std::launch::async, 
        [&value] () {
            // Simulate a long-running task
            std::this_thread::sleep_for(std::chrono::seconds(2));
            // Call the callback with the result
            onTaskComplete(value);
        });

    // Do other work while the task is running
    std::cout << "Doing other work..." << std::endl;

    // Wait for a key press to keep the program running
    std::cin.get();

    return 0;
}


 */