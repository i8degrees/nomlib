#include <cerrno>
#include <iostream>
#include "nomlib/core.hpp"
#include "nomlib/ptree.hpp"
#include "nomlib/serializers.hpp"
#include "nomlib/system.hpp"

using namespace nom;
using namespace std;

// const std::vector<std::string> filenames;
// filenames.push_back([
//   "auctions.json",
//   "inventory.json",
//   "omnom.json"
// ]);

Path p;
File cwd;
struct ProgramOpts {
  std::string root_path = cwd.currentPath();
  std::string self_path;
  std::string filename;
};

// NOM_EXPORT
std::vector<std::string> init_cmdline(int argc, char** argv) {
  std::vector<std::string> args;
  int num_args = argc;
  for(auto idx = 0; idx != num_args; ++idx) {
    const char* arg = argv[idx];
    if(arg != nullptr) {
      args.push_back(arg);
    }
  }

  return args;
}
namespace nom {

volatile uint64 rdtsc()
{
  uint32 a = 0.0f;
  uint32 d = 0.0f;
  asm volatile
      (".byte 0x0f, 0x31 #rdtsc\n" // edx:eax
       :"=a"(a), "=d"(d)::);
  return( ( (uint64) d) << 32) | (uint64) a;
}

} // namespace

int main(int argc, char** argv) {
  Value json;
  IValueDeserializer* fp; //= new JsonCppDeserializer();
  IValueSerializer* fpout; //= new JsonCppSerializer();
  nom::uint32 flags = 0;

  if(nom::init_third_party(flags) == false) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Could not initialize nomlib.");
    return(NOM_EXIT_FAILURE);
  }

  // Set the current working directory
  // if(nom::init(argc, argv) == false) {
  //   NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Could not initialize nomlib.");
  //   return(NOM_EXIT_FAILURE);
  // }

  ProgramOpts opts;
  std::vector<std::string> args = init_cmdline(argc, argv);

  if(args.size() > 1) {
    opts.self_path = args.at(0);
    opts.filename = args.at(1);
    std::cout << opts.filename << endl;
  } else {
    NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "A valid file path is required to continue.");
    return(ENOENT);
  }

  fp = new JsonCppDeserializer();
  if(!fp) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Failed to initialize deserializer. Out of memory?");
    return(ENOMEM);
  }

  fpout = new JsonCppSerializer(nom::SerializerOptions::HumanFriendly);
  if(!fpout) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Failed to initialize serializer. Out of memory?");
    return(ENOMEM);
  }
  // !! If we do not set the logging priority and trace priority in the next two
  // !! calls below, our execution time doubles; from ~10s to ~18s or more!
  nom::SDL2Logger::set_logging_priority(  NOM_LOG_CATEGORY_TRACE,
                                          nom::NOM_LOG_PRIORITY_INFO );
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_INFO);
  bool res;
  std::cout << nom::timestamp() << "\n\t hires_ticks: " << hires_ticks() << endl;
  auto d00 = nom::timestamp();
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE);
  res = fp->load(opts.filename, json);
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE);
  auto d01 = nom::timestamp();
  std::cout << "d1: " << d00 << " " << "d2: " << d01;

  if(!res) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_ERROR, "Failed to de-serialize JSON data!");
    return(EIO);
  }

  std::cout << json;

  auto d10 = nom::timestamp();
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE);
  res = fpout->save(json, "testme.json");
  auto d11 = nom::timestamp();
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE);
  std::cout << "d1: " << d10 << " " << "d2: " << d11;

  if(!res) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_ERROR, "Failed to serialize JSON data!");
    return(EIO);
  }

  nom::quit();
}
