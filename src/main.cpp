#include <iostream>
#include <thread>

#include "../include/controllers/person_controller.hpp"
#include "../include/router.hpp"
#include "../include/server.hpp"
#include "../include/services/person_service.hpp"

int main(void) {
  try {
    auto router = std::make_shared<Router>();
    //auto server = ThreadDetachServer(6969, router);
    auto server = AsyncServer(6969, router);
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

    // Simulate server running for some time
    //std::this_thread::sleep_for(std::chrono::seconds(3));

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



lambda

#include <iostream>
#include <functional>

void processNumbers(std::function<int(int)> callback) {
    int result = callback(8);
    std::cout << "Result: " << result << std::endl;
}

或

using Callback = std::function<void(int)>;

void processNumbers(Callback callback) {...}


int main()
{
    
    int m = 1;
    int n = 2;
    int *pn = &n;
    auto func1 = [&m, pn] (int a) {
            return m + *pn + a;
        };
   std::cout << func1(4) << std::endl << m << std::endl << n << std::endl;
   
   processNumbers(func1);

    std::cout<<"Hello World";

    return 0;
}


class Session : public std::enable_shared_from_this<Session> {

        auto self(shared_from_this());
        auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
        http::async_write(socket_, *sp,
          [this, self, sp](beast::error_code ec, std::size_t) {
            //socket_.shutdown(tcp::socket::shutdown_send, ec);
            socket_.close();
        });

        this 跟 self 就可以使用 class 的成員 , sp 需要輸入





async method with callback

#include <iostream>
#include <functional> 
#include <future> // async
#include <thread>

void onTaskComplete(int result) {
    std::cout << "Task completed with result: " << result << std::endl;
}


int main()
{
 
    int value = 10;

 
    // Perform the task asynchronously
    std::async(std::launch::async, 
        [&value] {
            // Simulate a long-running task
            std::this_thread::sleep_for(std::chrono::seconds(2));
            int result = value * 2; // Example computation
    
            // Call the callback with the result
            onTaskComplete(result);
        });


    // Do other work while the task is running
    std::cout << "Doing other work..." << std::endl;

    // Wait for a key press to keep the program running
    std::cin.get();
 
    return 0;
}
 */