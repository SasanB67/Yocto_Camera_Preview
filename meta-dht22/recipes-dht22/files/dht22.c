/*
 * dht22: read DHT22 temperature (default kernel dht11 driver)
 * and turn the LED on GPIO17 on (via libgpiod) when it is above the threshold.
 * The temperature file contains millidegrees C (25000 = 25.0 C).
 */
#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TEMP_FILE "/sys/bus/iio/devices/iio:device0/in_temp_input"
#define GPIOCHIP  "/dev/gpiochip0"
#define LED_PIN   17
#define THRESHOLD 20.0   /* degrees C: LED ON above this */

int main(void)
{
	unsigned int pin = LED_PIN;

	
	const char *temp_file = getenv("DHT22_TEMP_FILE");
	if (!temp_file)
		temp_file = TEMP_FILE;

	
	struct gpiod_chip *chip = gpiod_chip_open(GPIOCHIP);
	if (!chip) {
		perror(GPIOCHIP);
		return 1;
	}

	struct gpiod_line_settings *settings = gpiod_line_settings_new();
	gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
	gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

	struct gpiod_line_config *line_cfg = gpiod_line_config_new();
	gpiod_line_config_add_line_settings(line_cfg, &pin, 1, settings);

	struct gpiod_request_config *req_cfg = gpiod_request_config_new();
	gpiod_request_config_set_consumer(req_cfg, "dht22-led");

	struct gpiod_line_request *led = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
	gpiod_request_config_free(req_cfg);
	gpiod_line_config_free(line_cfg);
	gpiod_line_settings_free(settings);
	gpiod_chip_close(chip);
	if (!led) {
		perror("request GPIO17");
		return 1;
	}

	while (1) {
		FILE *f = fopen(temp_file, "r");
		int milli;

		
		if (f && fscanf(f, "%d", &milli) == 1) {
			float temp = milli / 1000.0;
			int on = temp > THRESHOLD;

			gpiod_line_request_set_value(led, LED_PIN,
				on ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
		}
		if (f)
			fclose(f);

		sleep(2);
	}
	return 0;
}
