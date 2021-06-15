//Libraries
#include <Adafruit_PWMServoDriver.h> // https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
#include <Wire.h>							 // https://www.arduino.cc/en/reference/wire

//Constants
#define nbPCAServo 16

//Parameters
int MIN_IMP[nbPCAServo] = { 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500 };
int MAX_IMP[nbPCAServo] = { 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500 };
int MIN_ANG[nbPCAServo] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int MAX_ANG[nbPCAServo] = { 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180 };

//Objects
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver( 0x40 );

void setup()
{
	//Init Serial USB
	Serial.begin( 9600 );
	Serial.println( F( "Initialize System" ) );
	pca.begin();
	pca.setPWMFreq( 60 );			  // Analog servos run at ~60 Hz updates
	pinMode( LED_BUILTIN, OUTPUT ); // Initialize the LED_BUILTIN pin as an output
}

void loop()
{
	pcaScenario();
}

void pcaScenario()
{
	//Scenario to test servomotors controlled by PCA9685 I2C Module
	for( int i = 0; i < nbPCAServo; i++ )
	{
		if( i == 0 )
		{
			Serial.println( "Turning on the LED" );
			digitalWrite( LED_BUILTIN, LOW ); // Turn the LED on
		}

		Serial.print( "Servo # " );
		Serial.println( i );
		//int middleVal=((MAX_IMP[i]+MIN_IMP[i])/2)/20000*4096; // conversion µs to pwmval
		//pca.setPWM(i,0,middleVal);
		for( int pos = ( MAX_IMP[i] + MIN_IMP[i] ) / 2; pos < MAX_IMP[i]; pos += 10 )
		{
			pca.writeMicroseconds( i, pos );
			delay( 10 );
		}
		for( int pos = MAX_IMP[i]; pos > MIN_IMP[i]; pos -= 10 )
		{
			pca.writeMicroseconds( i, pos );
			delay( 10 );
		}
		for( int pos = MIN_IMP[i]; pos < ( MAX_IMP[i] + MIN_IMP[i] ) / 2; pos += 10 )
		{
			pca.writeMicroseconds( i, pos );
			delay( 10 );
		}
		pca.setPin( i, 0, true );			  // deactivate pin i
		digitalWrite( LED_BUILTIN, HIGH ); // Turn the LED off
		if( i == 0 )
			Serial.println( "Turning off the LED" );
	}
} // End of pcaScenario() function.

int jointToImp( double x, int i )
{
	//Convert joint angle into pwm command value
	int imp = ( x - MIN_ANG[i] ) * ( MAX_IMP[i] - MIN_IMP[i] ) / ( MAX_ANG[i] - MIN_ANG[i] ) + MIN_IMP[i];
	imp = max( imp, MIN_IMP[i] );
	imp = min( imp, MAX_IMP[i] );
	return imp;
} // End of jointToImp() function.
