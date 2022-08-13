## Digispark Tiny85 and relay module as a time relay controller

### The story

Some time ago I bought a `Hikvision` video intercom, model `DS-KIS604-S(B)` and I'm using it to control my sliding gate.
The intercom allows me to open/close the gate using:
* mobile app (remote aspect) - `HikConnect`
* indoor station `DS-KH6320-WTE1`
* button

I've connected door station `DS-KV8113-WME1` to `Nice Robus 600` gate automation, through sockets `NO2` & `COM2` with `SBS` (step-by-step) socket.
First test. Opening sliding gate was fine. But when I triggered to close, the gate started closing. After ~20cm, the gate stopped and started opening again :worried:.

### The issue

Opening the gate through HikConnect door station was fine, but closing the gate was a tricky part. Door station closes the circuit at least for 1 second! And couldn't be decreased! The time resolution for Hikvision is in seconds.

On some portal I've read that `SBS` socket is fine for closed circuit within time range between `0.4` - `0.8` second.

### Solution

To workaround this issue, I've picked an old `Digispark Tiny85` and relay module `HW-307` and built cheap _time relay_.
Probably there are better solutions, but at that time I had those parts at home.

The idea is simple, the door station closes circuit on the Digispark and it triggers relay module to close `SBS` circuit in gate automation.

### Connections

|Digispark                        | Relay module | DS-KV8113-WME1 | Nice Robus 600 | Other                            |
|-------------------------------- | ------------ | -------------- | -------------- | -------------------------------- |
|5V                               | 5V           |                |                |                                  |
|GND                              | GND          |                |                |                                  |
|P0                               | IN           |                |                |                                  |
|5V                               |              | COM2           |                |                                  |
|GND with 10k Ohm resistor to P2* |              |                |                |                                  |
|P2                               |              | NO2            |                |                                  |
|VIN                              |              |                |                | Positive pole of power supply ** |
|GND                              |              |                |                | Negative pole of power supply    |
|                                 | NO           |                | SBS 1st slot   |                                  |
|                                 | COM          |                | SBS 2nd slot   |                                  |

\* GND - to one leg of 10k Ohm resistor, to second leg connect P2 pin and NO2 from door station.

\*\* VIN - in my Digispark there is `78L05` voltage regulator which accepts 30V maximum input voltage, but I've connected to 12V power supply which is also used for electric strike.

### Photos

The photos can be found [here](/docs/pictures).

### Useful materials and examples that project base on it

* Arduino build-in example [Debounce on a Pushbutton](https://docs.arduino.cc/built-in-examples/digital/Debounce)
* [#analogRead and resolutions](https://www.arduino.cc/reference/en/language/functions/analog-io/analogread)


## License

[License - Apache License 2.0](/LICENSE)

## Disclaimer

The information provided on this project is true and complete to the best of our knowledge. However, like various Do-It-Yourself (DIY) projects this DIY Project is at your own risk only.
As with any DIY project, lack of familiarity with the tools and process can be dangerous. The author/contributors are not liable for any damage or injury resulting from the assembly or use of this project including but not limited to hardware damage, body injuries etc.

You have been warned.
