#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class AddressBook
{
public:
  // using a template allows us to ignore the differences between functors, function pointers
  // and lambda
  template <typename Func>
  std::vector<std::string> findMatchingAddresses(Func func)
  {
    std::vector<std::string> results;
    for (auto itr = _addresses.begin(), end = _addresses.end(); itr != end; ++itr)
    {
      // call the function passed into findMatchingAddresses and see if it matches
      if (func(*itr))
      {
        results.push_back(*itr);
      }
    }
    return results;
  }

  void init()
  {
    _addresses.push_back("1.org");
    _addresses.push_back("2.org");
    _addresses.push_back("3.org");
    _addresses.push_back("1.com");
  };

private:
  std::vector<std::string> _addresses;
};

AddressBook global_address_book;

vector<string> findAddressesFromOrgs()
{
  return global_address_book.findMatchingAddresses(
      // we're declaring a lambda here; the [] signals the start
      [](const string &addr) { return addr.find(".org") != string::npos; });
}

vector<string> findAddressesFromName(const string &name)
{
  return global_address_book.findMatchingAddresses(
      // notice that the lambda function uses the the variable 'name'
      [&](const string &addr) { return addr.find(name) != string::npos; });
}

int main()
{
  global_address_book.init();

  for (auto address : findAddressesFromOrgs())
  {
    cout << address << endl;
  }

  cout << endl;

  for (auto address : findAddressesFromName("1"))
  {
    cout << address << endl;
  }

  vector<int> v;
  v.push_back(1);
  v.push_back(2);

  for_each(v.begin(), v.end(), [](int val) {
    cout << val << endl;
  });

  return 0;
}
