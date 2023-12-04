#ifndef WEB_STREAM
#define WEB_STREAM

#include <eloquent_esp32cam.h>
#include <eloquent_esp32cam/extra/exception.h>
#include <eloquent_esp32cam/extra/esp32/wifi/sta.h>
#include <eloquent_esp32cam/extra/esp32/http/server.h>

#define PORT 81



using namespace eloq;
using Eloquent::Extra::Exception;
using Eloquent::Extra::Esp32::Http::HttpServer;

namespace YCRCAR {
    namespace Esp32{
        namespace Cam {
            /**
             * Stream MJPEG video to browser
             */
            class WebStream {
                public:
                    Exception exception;
                    HttpServer server;

                    /**
                     * Constructor
                     */
                    WebStream() :
                        exception("WebStreamServer"),
                        server("WebStreamServer", PORT) {

                        }

                    /**
                     * Debug self IP address
                     */
                    String address() const {
                        return String("web stream is available at http://") + wifi.ip() + ":" + String(PORT);
                    }

                    /**
                     * Start server
                     */
                    Exception& begin() {
                        if (!wifi.isConnected())
                            return exception.set("WiFi not connected");

                        onJpeg();
                        onMjpeg();

                        // run in thread
                        server.thread.withStackSize(5000);

                        if (!server.begin().isOk())
                            return exception.propagate(server);

                        return exception.clear();
                    }

                protected:

                    /**
                     * Register / endpoint to get Mjpeg stream
                     */
                    void onMjpeg() {
                        server.onGET("/", [this](WebServer *web) {
                            WiFiClient client = web->client();

                            client.println(F("HTTP/1.1 200 OK"));
                            client.println(F("Content-Type: multipart/x-mixed-replace;boundary=frame"));
                            client.println(F("Access-Control-Allow-Origin: *"));
                            client.println(F("\r\n--frame"));
                            Serial.println("\n[Cam Client connected]");
                            while (client.connected() && camera.capture().isOk()) {
                                client.print("Content-Type: image/jpeg\r\nContent-Length: ");
                                client.println(camera.frame->len);
                                client.println();
                                client.write((const char *) camera.frame->buf, camera.frame->len);
                                client.println(F("\r\n--frame"));
                                client.flush();
                                delay(1);
                                yield();
                            }
                            Serial.println("[Cam Client disconnected]");
                            
                        });
                    }

                    /**
                     * Register /jpeg endpoint to get a single Jpeg
                     */
                    void onJpeg() {
                        server.onGET("/jpeg", [this](WebServer *web) {
                            if (!camera.capture().isOk())
                                return server.serverError(camera.exception.toString());

                            WiFiClient client = web->client();

                            client.println(F("HTTP/1.1 200 OK"));
                            client.println(F("Content-Type: image/jpeg"));
                            client.println(F("Access-Control-Allow-Origin: *"));
                            client.print(F("Content-Length: "));
                            client.println((unsigned int) camera.frame->len);
                            client.println();
                            client.write((const char *) camera.frame->buf, camera.frame->len);
                        });
                    }
            };
        }
    }
}

namespace yccy {
    namespace car {
        static YCRCAR::Esp32::Cam::WebStream webstream;
    }
}

#endif