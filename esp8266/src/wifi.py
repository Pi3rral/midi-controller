def connect_home_wifi():
    import network
    from utime import sleep_ms

    sta_if = network.WLAN(network.STA_IF)
    ap_if = network.WLAN(network.AP_IF)

    print("Trying to connect to home network...")
    sta_if.active(True)
    sleep_ms(500)
    sta_if.connect("WiGoat", "GoatFather")
    for i in range(0, 15):
        print(".")
        sleep_ms(1000)
        if sta_if.isconnected():
            print("Connected to home: {}".format(sta_if.ifconfig()[0]))
            ap_if.active(False)
            return
    sta_if.active(False)
    ap_if.active(True)
    print("Could not connect to home, Creating MiGoat access point")
    ap_if.config(essid="MiGoat", password="GoatFather")
    sleep_ms(5000)


def no_wifi():
    import network

    sta_if = network.WLAN(network.STA_IF)
    ap_if = network.WLAN(network.AP_IF)
    print("Disabling all Wifi access")
    sta_if.active(False)
    ap_if.active(False)
