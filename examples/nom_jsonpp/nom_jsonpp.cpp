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
  std::string input_filename = "";
  std::string output_filename = "";
  bool input_diag_print = true;
  // FIXME(JEFF):...
  bool output_overwrite_filename = true;
  nom::SerializerOptions output_options =
    nom::SerializerOptions::HumanFriendly;
    //nom::SerializerOptions::Compact;
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

int main(int argc, char** argv) {
  Value json;
  IValueDeserializer* fp; //= new JsonCppDeserializer();
  IValueSerializer* fpout; //= new JsonCppSerializer();
  nom::uint32 flags = 0;

  // Set the current working directory
  // if(nom::init(argc, argv) == false) {
  //   NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Could not initialize nomlib.");
  //   return(NOM_EXIT_FAILURE);
  // }

  ProgramOpts opts;
  std::vector<std::string> args =
    init_cmdline(argc, argv);

  //if(args.size() == 3) {
  //}

  // required
  if(args.size() > 0) {
    opts.self_path = args.at(0);
  } else {
    NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "There should always be one argument.");
    return(ENOENT);
  }

  // required
  if(args.size() > 1) {
    opts.input_filename = args.at(1);
    // FIXME(JEFF):check for file existence?
    std::cout << opts.input_filename << endl;
  } else {
    NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "A valid input file path is required for reading.");
    return(ENOENT);
  }

  // optional
  // FIXME(JEFF):never overwrite existing files! add check
  if(args.size() > 2) {
    opts.output_filename = args.at(2);
    std::cout << opts.output_filename << endl;
  }

  fp = new JsonCppDeserializer(opts.output_options);
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
  res = fp->load(opts.input_filename, json);
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE);
  auto d01 = nom::timestamp();
  std::cout << "d1: " << d00 << " " << "d2: " << d01;

  if(!res) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_ERROR, "Failed to de-serialize JSON data!");
    return(EIO);
  }

  if(opts.input_diag_print == true) {
    std::cout << json;
  }

  auto d10 = nom::timestamp();
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE);
  if(opts.output_overwrite_filename) {
    if(opts.output_filename != "") {
      res = fpout->save(json, opts.output_filename);
    }
  } else if(opts.output_overwrite_filename == false) {
    // FIXME(JEFF):implement opt check here; we should never overwrite existing files
    // FIXME(JEFF):by default!
  }

  auto d11 = nom::timestamp();
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE);
  std::cout << "d1: " << d10 << " " << "d2: " << d11;

  if(!res) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_ERROR, "Failed to serialize JSON data!");
    return(EIO);
  }

  nom::quit();
}
