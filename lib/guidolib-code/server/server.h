/*

 Copyright (C) 2012 Grame

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
 research@grame.fr

 */


#ifndef __HTTPDServer__
#define __HTTPDServer__

#include <string>
#include <ostream>
#include <vector>
#include <map>
#include <microhttpd.h>

#include "guidosession.h"
#include "guido2img.h"

// json
#include "json/json_object.h"
#include "json/json_array.h"
#include "json/json_parser.h"

#define IP_VERBOSE 1
#define HEADER_VERBOSE 2
#define REQUEST_VERBOSE 4
#define URL_VERBOSE 8
#define QUERY_VERBOSE 16
#define CODE_VERBOSE 32
#define MIME_VERBOSE 64
#define LENGTH_VERBOSE 128

#define HEAD 3
#define DELETE 2
#define POST 1
#define GET 0

namespace guidohttpd
{

class guido2img;
//--------------------------------------------------------------------------
/*!
 \brief a specific thread to listen incoming osc packets
 */

struct connection_info_struct {
    int connectiontype;
    TArgs args;
    struct MHD_PostProcessor *postprocessor;
};
class HTTPDServer
{
    int fVerbose;
    int fLogmode;
    string fCachedir;
    string fSvgFontFile;
    struct MHD_Daemon *	fServer;
    guido2img* fConverter;
    int fMaxSessions;
    std::map<std::string, guidosession *> fSessions;

    const char* getMIMEType (const std::string& page);

public:

    HTTPDServer(int verbose, int logmode, string cachedir, string svgfontfile, guido2img* g2img);
    virtual ~HTTPDServer();

    /// \brief starts the httpd server
    bool start (int port);
    void stop ();
    int answer (struct MHD_Connection *connection, const char *url, const char *method, const char *version,
                const char *upload_data, size_t *upload_data_size, void **con_cls);
    int sendGuido (struct MHD_Connection *connection, const char* url, const TArgs& args, int type);
    int sendGuidoPostRequest (struct MHD_Connection *connection, const TArgs& args);
    int sendGuidoDeleteRequest (struct MHD_Connection *connection, const TArgs& args);

    guidosessionresponse registerGMN(string unique_id, string gmn);
    void readFromCache(string target = "");

    int send (struct MHD_Connection *connection, guidosessionresponse &response);
    int send (struct MHD_Connection *connection, const char *page, int length, const char *type, int status=MHD_HTTP_OK);
    int send (struct MHD_Connection *connection, const char *page, const char *type, int status=MHD_HTTP_OK);

};

} // end namespoace

#endif
