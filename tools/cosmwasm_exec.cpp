#include <serdepp/serde.hpp>
#include <serdepp/adaptor/nlohmann_json.hpp>

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
   // example of a host "method"
   void print_name(const char* nm) { std::cout << "Name : " << nm << " " << field << "\n"; }
   // example of another type of host function
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

   uint32_t __wbindgen_string_new(uint32_t num1, uint32_t num2) { return num1 + num2; }

   void abort(int32_t num) {
      std::cout << "Abort: " << num << "\n";
      throw 0;
   }

   uint32_t db_read(uint32_t num) { return num; }
   void db_write(uint32_t num1, uint32_t num2) { std::cout << "db_write: " << num1 << " " << num2 << "\n"; }
   void db_remove(uint32_t num) { std::cout << "db_remove: "<< num << "\n"; }
   uint32_t db_scan(uint32_t num1, uint32_t num2, int32_t num3) { return num1; }
   uint32_t db_next(uint32_t num) { return num; }
   uint32_t addr_validate(uint32_t num) { return num; }
   uint32_t addr_canonicalize(uint32_t num1, uint32_t num2) { return num1 + num2; }
   uint32_t addr_humanize(uint32_t num1, uint32_t num2) { return num1 + num2; }
   uint32_t secp256k1_verify(uint32_t num1, uint32_t num2, uint32_t num3) { return num1 + num2 + num3; }
   uint64_t secp256k1_recover_pubkey(uint32_t num1, uint32_t num2, uint32_t num3) { return num1 + num2 + num3; }
   uint32_t ed25519_verify(uint32_t num1, uint32_t num2, uint32_t num3) { return num1 + num2 + num3; }
   uint32_t ed25519_batch_verify(uint32_t num1, uint32_t num2, uint32_t num3) { return num1 + num2 + num3; }
   void debug(uint32_t num) { std::cout << "debug: "<< num << "\n"; }
   uint32_t query_chain(uint32_t num) { return num; }

   void __wbindgen_describe(int32_t num) { std::cout << "__wbindgen_describe: " << num << "\n"; }
   void __wbg_log_7bb108d119bafbc1(int32_t num) { std::cout << "__wbg_log_7bb108d119bafbc1: " << num << "\n"; }
   void __wbindgen_object_drop_ref(int32_t num) { std::cout << "__wbindgen_object_drop_ref: " << num << "\n"; }
   void __wbindgen_throw(uint32_t num1, uint32_t num2) { std::cout << "__wbindgen_throw: " << num1 << " " << num2 << "\n"; }
   uint32_t __wbindgen_externref_table_grow(uint32_t num) { return num; }
   void __wbindgen_externref_table_set_null(int32_t num) { std::cout << "__wbindgen_externref_table_set_null: " << num << "\n"; }

   int32_t instantiate(int32_t num1, int32_t num2, int32_t num3) {
      std::cout << num1 << " " << num2 << " " << num3 << '\n';
      return num1 + num2 + num3;
   }
};

struct cnv : type_converter<example_host_methods> {
   using type_converter::type_converter;
   using type_converter::from_wasm;
   EOS_VM_FROM_WASM(bool, (uint32_t value)) { return value ? 1 : 0; }
   EOS_VM_FROM_WASM(char*, (void* ptr)) { return static_cast<char*>(ptr); }
   EOS_VM_FROM_WASM(const char*, (void* ptr)) { return static_cast<char*>(ptr); }
};

// EOS_VM_PRECONDITION(test_name,
//       EOS_VM_INVOKE_ON(const char*, [&](auto&& nm, auto&&... rest) {
//          std::string s = nm;
//          if (s == "eos-vm2")
//             throw "failure";
//    }))

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

   // main function
   rhf_t::add<&example_host_methods::abort>("env", "abort");
   // rhf_t::add<&example_host_methods::instantiate>("env", "instantiate");
   rhf_t::add<&example_host_methods::db_read>("env", "db_read");
   rhf_t::add<&example_host_methods::db_write>("env", "db_write");
   rhf_t::add<&example_host_methods::db_remove>("env", "db_remove");
   rhf_t::add<&example_host_methods::db_scan>("env", "db_scan");
   rhf_t::add<&example_host_methods::db_next>("env", "db_next");
   rhf_t::add<&example_host_methods::addr_validate>("env", "addr_validate");
   rhf_t::add<&example_host_methods::addr_canonicalize>("env", "addr_canonicalize");
   rhf_t::add<&example_host_methods::addr_humanize>("env", "addr_humanize");
   rhf_t::add<&example_host_methods::secp256k1_verify>("env", "secp256k1_verify");
   rhf_t::add<&example_host_methods::secp256k1_recover_pubkey>("env", "secp256k1_recover_pubkey");
   rhf_t::add<&example_host_methods::ed25519_verify>("env", "ed25519_verify");
   rhf_t::add<&example_host_methods::ed25519_batch_verify>("env", "ed25519_batch_verify");
   rhf_t::add<&example_host_methods::debug>("env", "debug");
   rhf_t::add<&example_host_methods::query_chain>("env", "query_chain");

   // debugging build function
   rhf_t::add<&example_host_methods::__wbindgen_string_new>("__wbindgen_placeholder__", "__wbindgen_string_new");
   rhf_t::add<&example_host_methods::__wbindgen_describe>("__wbindgen_placeholder__", "__wbindgen_describe");
   rhf_t::add<&example_host_methods::__wbg_log_7bb108d119bafbc1>("__wbindgen_placeholder__", "__wbg_log_7bb108d119bafbc1");
   rhf_t::add<&example_host_methods::__wbindgen_object_drop_ref>("__wbindgen_placeholder__", "__wbindgen_object_drop_ref");
   rhf_t::add<&example_host_methods::__wbindgen_throw>("__wbindgen_placeholder__", "__wbindgen_throw");
   rhf_t::add<&example_host_methods::__wbindgen_externref_table_grow>("__wbindgen_externref_xform__", "__wbindgen_externref_table_grow");
   rhf_t::add<&example_host_methods::__wbindgen_externref_table_set_null>("__wbindgen_externref_xform__", "__wbindgen_externref_table_set_null");

   watchdog wd{std::chrono::seconds(3)};

   try {
      example_host_methods ehm;
      // Instaniate a new backend using the wasm provided.

      std::string filename = argv[1];

      auto code = read_wasm( filename );
      backend_t bkend(code, ehm, &wa);

      // Instaniate a "host"
      ehm.field = "testing";

      // serialize json params
      auto test_env = serde::parse_file<nlohmann::json>("./test_env.json");
      auto test_info = serde::parse_file<nlohmann::json>("./test_info.json");
      auto test_instantiate_msg = serde::parse_file<nlohmann::json>("./test_instantiate_msg.json");

      // Execute "instantiate" instead of "apply".
      auto ret = bkend.call_with_return(
         ehm, "env", "instantiate",
         test_env.get_binary(),
         test_info.get_binary(),
         test_instantiate_msg.get_binary()
      );

      std::cout << ret.value().to_i32() << '\n';

   } catch (const eosio::vm::exception& ex) {
      std::cerr << "eos-vm interpreter error\n";
      std::cerr << ex.what() << " : " << ex.detail() << "\n";
      return 1;
   }

   std::cout << "Cosmwasm contract is successfully executed on EOSVM!!" << '\n';

   return 0;
}
