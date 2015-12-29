#include <ACNoblex.h>
#include <IRremote.h>

IRsend irsend;
ACNoblex ac(&irsend);

char r;

typedef void (*command_cb)(void);

typedef struct {
	command_cb callback;
	char mnemonic;
} command_t;

void temp_up();
void temp_down();
void power();
void light();
void mode();
void fan();
void swing();
void status();

command_t commands[] = {
	{power, 	'p'},
	{light, 	'l'},
	{mode, 		'm'},
	{fan, 		'f'},
	{temp_up, 	'T'},
	{temp_down, 't'},
	{swing, 	'S'},
	{status, 	's'},
	NULL
};


void setup() {
	Serial.begin(115200);
	while (!Serial);

	Serial.println("SimpleIR initialized!");
}

void loop() {
	static unsigned int i;
	static bool found;

	Serial.print("ACNoblex> ");

	while (Serial.available() <= 0);
	r = Serial.read();

	found = false;
	for (i = 0; i < (sizeof(commands) / sizeof(commands[0])); i++) {
		if (commands[i].mnemonic  == r) {
			found = true;
			commands[i].callback();
			ac.sendCommand();
			status();
			break;
		}
	}

	if (!found) {
		Serial.println();
		Serial.print(r);
		Serial.println(": unknown command! Usage:");
		Serial.println("\tp: switch power on/off");
		Serial.println("\tl: switch light on/off");
		Serial.println("\tT: temperature up");
		Serial.println("\tt: temperature up");
		Serial.println("\tm: switch modes (fan, cool, etc)");
		Serial.println("\tf: switch fan speed (min, med, etc)");
		Serial.println("\ts: print AC status");
	}
}

void power() {
	Serial.println("POWER!");
	if (ac.isOn()) {
		ac.off();
	} else {
		ac.on();
	}
}

void light() {
	Serial.println("LIGHT!");
	ac.setLight(!ac.getLight());
}

void mode() {
	Serial.println("MODE!");
	switch (ac.getMode()) {
	case MODE_AUTO:
		ac.setMode(MODE_HEAT);
		break;
	case MODE_HEAT:
		ac.setMode(MODE_DRY);
		break;
	case  MODE_DRY:
		ac.setMode(MODE_COOL);
		break;
	case  MODE_COOL:
		ac.setMode(MODE_FAN);
		break;
	case  MODE_FAN:
		ac.setMode(MODE_AUTO);
		break;
	}
}

void fan() {
	Serial.println("FAN!");
	switch (ac.getFanSpeed()) {
	case FAN_AUTO:
		ac.setFanSpeed(FAN_MED);
		break;
	case FAN_MED:
		ac.setFanSpeed(FAN_MIN);
		break;
	case FAN_MIN:
		ac.setFanSpeed(FAN_MAX);
		break;
	case FAN_MAX:
		ac.setFanSpeed(FAN_AUTO);
		break;
	}
}

void temp_up() {
	Serial.println("T-UP!");
	ac.setTemp(ac.getTemp() + 1);
}

void temp_down() {
	Serial.println("T-DOWN!");
	ac.setTemp(ac.getTemp() - 1);
}

void swing() {
	Serial.println("SWING!");
	ac.setSwing(!ac.getSwing());
}

void status() {
	Serial.println("STATUS!");
	Serial.print("Power: ");
	Serial.println(ac.isOn() ? "ON" : "OFF");
	Serial.print("Light: ");
	Serial.println(ac.getLight() ? "ON" : "OFF");
	Serial.print("Swing: ");
	Serial.println(ac.getSwing() ? "ON" : "OFF");
	Serial.print("Mode: ");
	switch (ac.getMode()) {
	case MODE_AUTO:
		Serial.println("MODE_AUTO");
		break;
	case MODE_COOL:
		Serial.println("MODE_COOL");
		break;
	case MODE_DRY:
		Serial.println("MODE_DRY");
		break;
	case MODE_HEAT:
		Serial.println("MODE_HEAT");
		break;
	case MODE_FAN:
		Serial.println("MODE_FAN");
		break;
	}
	Serial.print("Fan Speed: ");
	switch (ac.getFanSpeed()) {
	case FAN_AUTO:
		Serial.println("FAN_AUTO");
		break;
	case FAN_MIN:
		Serial.println("FAN_MIN");
		break;
	case FAN_MED:
		Serial.println("FAN_MED");
		break;
	case FAN_MAX:
		Serial.println("FAN_MAX");
		break;
	}
	Serial.print("Temperature: ");
	Serial.print(ac.getTemp());
	Serial.println(" °C");
}