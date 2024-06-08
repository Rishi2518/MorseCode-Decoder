#include <stdio.h> 
#include "pico/stdlib.h"
#include "time.h" 
#include "includes/seven_segment.h" 
#include <string.h> 

// Define GPIO pin for the button and a delay time
#define BUTTON_PIN 16  
#define DELAY_TIME 300

// Morse code symbols and array to store Morse code sequence
char morse[5]="";
char dot = '.';
char dash='-';

// Function to display characters on a seven-segment display based on Morse code symbols
void displayCharacter(char character) {
    if (character == '.'){
        seven_segment_show(27);  // Display Morse code dot symbol on the seven-segment display
    } else if (character == '-'){
        seven_segment_show(26);  // Display Morse code dash symbol on the seven-segment display
    }	
}

// Function to interpret Morse code and display the corresponding character on the seven-segment display
void out(char morse[5]) {
    // Morse code representations for each letter
    const char* letters[] = {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", 
        "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", 
        "..-", "...-", ".--", "-..-", "-.--", "--.."
    };

    bool error = true;

    // Loop through Morse code representations to find a match
    for (int i = 0; i < 26; ++i) {
        if (strcmp(morse, letters[i]) == 0) {
            printf("%c", 'A' + i);  // Print the corresponding character
            seven_segment_show(i);   // Display the corresponding character on the seven-segment display
            error = false;
            break;
        }
    }

    // Display an error message if Morse code is not recognized
    if (error) {
        printf("Input is not recognised");
        seven_segment_show(27);  // Display an error symbol on the seven-segment display
    }
}

int main() {
    // Initialization of standard input/output, GPIO, seven-segment display, and variables
    stdio_init_all(); 
    gpio_init(BUTTON_PIN); 
    gpio_set_dir(BUTTON_PIN, GPIO_IN); 
    seven_segment_init(); 
    seven_segment_off(); 

    uint64_t pressed_start_time = 0; 
    bool printed_long_press_message = false;

    printf("Welcome To Our Morse Code Program!\n"); 
    displayCharacter('-'); 
    sleep_ms(600); 
    seven_segment_show(26); 
    sleep_ms(600); 
    seven_segment_off(); 

    double current_time_1;
    double time_2;

    while (true) {
        // Main loop for reading button presses and interpreting Morse code

        if (gpio_get(BUTTON_PIN)) {
            // Code block executed when the button is pressed

            seven_segment_off(); 
            double current_time = to_ms_since_boot(get_absolute_time()); 
            bool a = gpio_get(BUTTON_PIN); 
            
            // Wait until the button is released, displaying Morse code dash symbol
            while(a){    
                a = gpio_get(BUTTON_PIN);
                seven_segment_show(26);
            }
            seven_segment_off();
            pressed_start_time = to_ms_since_boot(get_absolute_time());
            double duration = pressed_start_time - current_time;
            current_time_1 = to_ms_since_boot(get_absolute_time());

            if(duration < 250){
                // Code block executed for dot symbol
                strncat(morse, &dot, 1);
                printf("%s \n", morse);
            }
            if(duration > 250 ){
                // Code block executed for dash symbol
                strncat(morse, &dash, 1);
                printf("%s \n", morse);
            }
            if(duration > 900){
                // Display an error message for a long button press
                printf("ERROR: Pressed for too long");
                strcpy(morse,"");
                seven_segment_show(27);
            }
        }

        double time_2 = to_ms_since_boot(get_absolute_time());
        double duration_2 = time_2 - current_time_1;

        if(duration_2 > 900 && morse[0] != '\0') {
            // Interpret Morse code and display corresponding character
            out(morse);
            strcpy(morse, "");
        }   
        sleep_ms(20);
    }
}
