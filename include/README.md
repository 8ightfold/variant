# variant
This is my own implementation of ``std::variant`` for c++20. 
Instead of using a recursive union, it uses pointers to represent the various possible states.
It uses multiple inheritance of a "node" structure to call the active overload.

## Usage
``variant`` can be used very similarly to ``std::variant``. 
Here is an example of how it can be used:
```cpp
variant<int, std::string> v1 { "Hello world!" };
v1.visit([](auto&& a) { std::cout << a << std::endl; });

v1.emplace<int>(1);
std::cout << "at int: " << v1[Tp<int>] << std::endl;

variant v2 { std::move(v1) };
v2.visit([](auto&& a) { std::cout << a << std::endl; });

v2.emplace<std::string>("std::string is active!");
if(v2.contains<std::string>()) 
    std::cout << v2[Tp<std::string>] << std::endl;
```

## Notes
You cannot use multiple of the same type in the variant, 
as there are several overloads that would be ambiguous.

As always, if you have any issues, feel free to let me know!