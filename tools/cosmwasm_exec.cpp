#include <eosio/vm/backend.hpp>
#include <eosio/vm/error_codes.hpp>
#include <eosio/vm/host_function.hpp>
#include <eosio/vm/watchdog.hpp>

#include <iostream>
#include <string>

using namespace eosio;
using namespace eosio::vm;

struct example_host_methods {
   // void print_num(uint64_t n) { std::cout << "Number : " << n << "\n"; }
   // // example of a host "method"
   void print_name(const char* nm) { std::cout << "Name : " << nm << " " << field << "\n"; }
   // // example of another type of host function
   // void* memset(char* ptr, int x, size_t n) { return ::memset(ptr, x, n); }
   std::string  field = "";

   // void eosio_assert(bool test, const char* msg) {
   //    if (!test) {
   //       std::cout << msg << std::endl;
   //       throw 0;
   //    }
   // }

   // void print_span(span<const char> s) {
   //    std::cout << "Span : " << std::string{s.data(), s.size()} << "\n";
   // }

   void abort(int32_t num) { std::cout << "Abort: "<< num << "\n"; }
   int32_t db_read(int32_t num) { return num; }

   void instantiate() { std::cout << "Instantiate method triggered & executed on EOSVM from Cosmwasm contract!"; }
};

struct cnv : type_converter<example_host_methods> {
   using type_converter::type_converter;
   using type_converter::from_wasm;
   EOS_VM_FROM_WASM(bool, (uint32_t value)) { return value ? 1 : 0; }
   EOS_VM_FROM_WASM(char*, (void* ptr)) { return static_cast<char*>(ptr); }
   EOS_VM_FROM_WASM(const char*, (void* ptr)) { return static_cast<char*>(ptr); }
};

EOS_VM_PRECONDITION(test_name,
      EOS_VM_INVOKE_ON(const char*, [&](auto&& nm, auto&&... rest) {
         std::string s = nm;
         if (s == "eos-vm2")
            throw "failure";
   }))

/**
 * Simple implementation of an interpreter using eos-vm.
 */
int main(int argc, char** argv) {
   // Thread specific `allocator` used for wasm linear memory.
   wasm_allocator wa;
   // Specific the backend with example_host_methods for host functions.
   using rhf_t     = eosio::vm::registered_host_functions<example_host_methods, execution_interface, cnv>;
   using backend_t = eosio::vm::backend<rhf_t>;

   // register print_num
   // rhf_t::add<&example_host_methods::print_num>("env", "print_num");
   // // register eosio_assert
   // rhf_t::add<&example_host_methods::eosio_assert>("env", "eosio_assert");
   // // register print_name
   // rhf_t::add<&example_host_methods::print_name, test_name>("env", "print_name");
   // // finally register memset
   // rhf_t::add<&example_host_methods::memset>("env", "memset");
   // rhf_t::add<&example_host_methods::print_span>("env", "print_span");

   rhf_t::add<&example_host_methods::abort>("env", "abort");
   rhf_t::add<&example_host_methods::instantiate>("env", "instantiate");
   rhf_t::add<&example_host_methods::db_read>("env", "db_read");

   watchdog wd{std::chrono::seconds(3)};
   try {
      example_host_methods ehm;
      // Instaniate a new backend using the wasm provided.

      std::string filename = argv[1];

      auto code = read_wasm( filename );
      backend_t bkend(code, ehm, &wa);

      // Instaniate a "host"
      ehm.field = "testing";
      // Execute "instantiate" instead of "apply".
      bkend(ehm, "env", "instantiate");

   } catch (const eosio::vm::exception& ex) {
      std::cerr << "eos-vm interpreter error\n";
      std::cerr << ex.what() << " : " << ex.detail() << "\n";
   }

   return 0;
}
