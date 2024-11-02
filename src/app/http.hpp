#include <string>

// ####################
// HTTP
// ####################

using std::string;

namespace Chars {
  const int SP = 0x20; // 32 ' '
  const int CR = 0x0D; // 13 '\r'
  const int LF = 0x0A; // 10 '\n'
}

namespace Http {
  // METHOD
  class Method {
  public:
    static const string HEAD;
    static const string OPTIONS;
    static const string CONNECT;
    static const string TRACE;

    static const string GET;
    static const string POST;
    static const string PUT;
    static const string PATCH;
    static const string DELETE;

    static const int MAX_LENGTH;

  private:
    Method();
  };

  // VERSION
  class Version {
  public:
    const string LITTERAL;
    const int MAJOR;
    const int MINOR;

    static const Version v0_9;
    static const Version v1_0;
    static const Version v1_1;
    static const Version v2_0;
    static const Version v3_0;

  private:
    Version();
    Version(const string litteral, const int major, const int minor) : LITTERAL(litteral), MAJOR(major), MINOR(minor){};
  };

  // STATUS
  class Status {
  public:
    const int CODE;
    const string MESSAGE;

    // 400 client
    static const Status BAD_REQUEST;
    static const Status NOT_FOUND;
    static const Status METHOD_NOT_ALLOWED;
    static const Status URI_TOO_LONG;

    // 500 server
    static const Status INTERNAL_SERVER_ERROR;
    static const Status NOT_IMPLEMENTED;
    static const Status VERSION_NOT_SUPPORTED;

  private:
    Status();
    Status(const int code, const string msg) : CODE(code), MESSAGE(msg) {};
  };
}
