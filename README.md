### How to build OpenThread with Zephyr and WAMR

----

First install `west` according to the Zephyr's documentation(Ubuntu)

[]: https://docs.zephyrproject.org/latest/guides/west/install.html	"west guide"

```shell
pip3 install --user -U west
```

Then run `west init` in any location you want to install Zephyr:

```shell
cd /path/to/where/you/want
west init
```

Then go to Zephyr's folder, and modify the `west.yml`, replace the original openthread with the following link:

```
    - name: openthread
      url: https://github.com/oubotong/openthread
      revision: 1446709296968d23ab30470384c626303012bac3
      path: modules/lib/openthread
    - name: paho
      url: https://github.com/eclipse/paho.mqtt-sn.embedded-c
      revision: e64e817f80
```

Then run `west update` to update the new module to your Zephyr project. By far the module have been successfully updated in Zephyr.

The next step is to build environment for OpenThread:

```shell
cd modules/lib/openthread
./script/bootstrap
./bootstrap
```

Before the next step, you need to run the MQTT-SN gateway to connect your device to broker(your border router):

```shell
#On your border router
git clone -b experiment https://github.com/eclipse/paho.mqtt-sn.embedded-c   
cd paho.mqtt-sn.embedded-c/MQTTSNGateway       
make SENSORNET=udp6 
make install   
cp -pf Build/MQTT-SNGateway ../../
cp -pf Build/MQTT-SNLogmonitor ../../
cp -pf gateway.conf ../../
cp -pf clients.conf ../../
cp -pf predefinedTopic.conf ../../
make clean
```

Before running the gateway, we need to modify the configuration of the gateway which is `gateway.conf`

```shell
cd ../
vim gateway.conf

#Change the following configuration
BrokerName=mqtt.eclipse.org #Public broker address for testing
GatewayUDP6Broadcast=FF03::1
GatewayUDP6Port=47193
# UDP6
GatewayUDP6Bind=FFFF:FFFE::1 
GatewayUDP6Port=47193
GatewayUDP6Broadcast=FF03::1
GatewayUDP6If=wpan0 #the interface you use for the border router
```

To start the gateway, run:

```shell
./MQTT-SNGateway -f gateway.conf
```

Then you will see that the gateway is ready for listening.

Then copy the `Zephyr-WAMR` folder in `zephyr/samples/` and rename the folder to `webassembly`. The source file of the WASM runtime is in `src/main.c` which has exposed an interface to connect to the MQTT-SN gateway. The `src/test_wasm.h` files contains `wasm_test_file4`which represents the bytecode of wasm module which would access the `mqttsnConnect`interface.

To build this example, go to `zephyr/samples/webassembly` and run(for NRF52840-DK):

```shell
west build -b nrf52840dk_nrf52840   . -p always -- -DCONF_FILE="prf_nrf52840_dk.conf  overlay-ot.conf"  -DWAMR_BUILD_TARGET=THUMB    -DWAMR_BUILD_AOT=0
west flash
```

After a few minutes, you will see that on your border router, you can see the connect message which means the connection is established.

Then open a terminal and start to monitor the UART:

```shell
sudo minicom -D /dev/ttyACM0 -b 115200 #the port is which you have your board attached to
```

You will see that the device role change to router if you run `ot state` which means the device has joined the network.



