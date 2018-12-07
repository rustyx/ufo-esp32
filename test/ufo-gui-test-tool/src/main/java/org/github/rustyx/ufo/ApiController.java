package org.github.rustyx.ufo;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import lombok.Getter;

import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class ApiController {

    @Getter
    public static class Apis {
        List<String> apis = new ArrayList<String>();
    }

    private Apis apis = new Apis();;

    public ApiController() {
        apis.apis.add("/api?sample_1");
        apis.apis.add("/api?sample_2");
        apis.apis.add("/api?sample_3");
    }

    @RequestMapping(value = "/api", produces = "application/json")
    public String api() throws IOException {
        return "{}";
    }

    @RequestMapping("/apiedit")
    public ResponseEntity<String> apiedit(@RequestParam int apiid, @RequestParam String apiedit, @RequestParam(required = false) String delete)
            throws IOException {
        if (delete == null) {
            if (apiid >= apis.apis.size())
                apis.apis.add(apiedit);
            else
                apis.apis.set(apiid - 1, apiedit);
        } else {
            apis.apis.remove(apiid - 1);
        }
        return ResponseEntity.status(302).header("Location", "/").body("");
    }

    @RequestMapping("/apilist")
    public Apis apilist() throws IOException {
        return apis;
    }

    @RequestMapping(value = "/info", produces = "application/json")
    public String info() throws IOException {
        return "{\"apmode\":\"0\",\"heap\":135680,\"ssid\":\"test-ssid\",\"hostname\":\"UFO\",\"enterpriseuser\":\"\","
                + "\"sslenabled\":\"0\",\"listenport\":0,\"ipaddress\":\"1.2.3.4\",\"ipgateway\":\"2.3.4.5\","
                + "\"ipsubnetmask\":\"255.255.255.0\",\"rssi\":-81,\"channel\":1,\"macaddress\":\"24:01:02:03:04:05\","
                + "\"firmwareversion\":\"Dec32018-14:23:32\",\"ufoid\":\"ufo-id\",\"ufoname\":\"ufo-id\","
                + "\"organization\":\"\",\"department\":\"\",\"location\":\"\",\"dtenabled\":0,\"dtenvid\":\"\","
                + "\"dtinterval\":0,\"dtmonitoring\":0,\"adminpw\":\"test\",\"mqtturi\":\"mqtts://some-host:8883\","
                + "\"mqtttopic\":\"/topic/qos0\",\"mqttpw\":\".....\",\"mqttstatusperiodseconds\":66,"
                + "\"mqttstatusqos\":0,\"mqttqos\":1,\"mqttkeepalive\":33,"
                + "\"mqttservercert\":\"-----BEGIN CERTIFICATE-----\\ndata\\n-----END CERTIFICATE-----\\n\","
                + "\"mqttclientkey\":\"\",\"mqttclientcert\":\"\"}";
    }

    @RequestMapping(value = { "/config", "/srvconfig", "/mqttconfig" }, produces = "text/html")
    public String config() throws IOException {
        return "<html><head><meta http-equiv=\"refresh\" content=\"3; url=/\"></head><body>"
                + "<h2>New settings stored, rebooting shortly (simulation).</h2></body></html>";
    }

    @RequestMapping(value = "/firmware", produces = "application/json")
    public String firmware() throws IOException {
        return "{\"session\":\"1\",\"progress\":0,\"status\":\"off\"}";
    }

    @RequestMapping(value = "/checkfirmware", produces = "application/json")
    public String checkfirmware() throws IOException {
        return "{}";
    }

    @RequestMapping("/update")
    public String update() throws IOException {
        return "<html><head><meta http-equiv=\"refresh\" content=\"3; url=/\"></head><body>"
                + "<h2>Firmware update succeeded, rebooting shortly (simulation).</h2></body></html>";
    }

}
