/**
 * nesheader.c
 * Simple utility for creating 16 byte headers for use in .nes files
 * 
 * Created from documentation at https://wiki.nesdev.com/w/index.php/INES
 * 
 * Example Usage:
 *   $ ./nesheader -p128 -c128 -r0 -m1 -v > somegame.header
 * 
 * Make a usable rom:
 *   $ cat somegame.header somegame.bin > somegame.nes
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// TODO: Add comments to all functions.

/**
 * Create flag values for byte 6 of the INES header.
 * Args:
 *   mapper: int, INES mapper number, ex: 0 => NROM, 1 => MMC1, etc.
 * Returns: a byte containing flag data.
 */
char flag6(int mapper, int fourscreen, int trainer_present, int battery_backed, int vertical_mirror) {
    char output = mapper << 4;
    output = output | fourscreen << 3;
    output = output | trainer_present << 2;
    output = output | battery_backed << 1;
    output = output | vertical_mirror;
    return output;
}

char flag7() {
    return 0;
}

char flag9() {
    return 0;
}

void writeHeader(prgrom_kb, chrrom_kb, prgram_kb, mapper, four_screen, trainer_present, battery_backed, vertical_mirror) {
    // Bytes 0-3
    // First four bytes are always N,E,S, MS-DOS EOF.
    const char NES[4] = {0x4E, 0x45, 0x53, 0x1A};
    int i = 0;
    for(i = 0; i < 4; i++) {
        putchar(*(NES + i));
    }

    // Byte 4: PRGROM size divided by 16k.
    int prgrom_div_16k = prgrom_kb / 16;
    putchar(prgrom_div_16k);

    // Byte 5: CHRROM size divided by 8k.
    int chrrom_div_8k = chrrom_kb / 8;
    putchar(chrrom_div_8k);

    // Byte 6: Flag 6 values.
    putchar(flag6(mapper,four_screen,trainer_present,battery_backed,vertical_mirror));

    // Byte 7: Flag 7 values.
    putchar(flag7());

    // Byte 8: PRGRAM size divided by 8k.
    int prgram_div_8k = prgram_kb / 8;
    putchar(prgram_div_8k);

    // Byte 9: Flag 9 values.
    putchar(flag9());

    // Byte 10-15: Zero-filled.
    for (i=0; i < 6; i++) {
        putchar(0);
    }
}

void help() {
    // TODO: Use single multi-line string.
    // TODO: Look at gcc --help for formatting.
    printf("Example Usage:\n");
    printf("$ ./nesheader -p128 -c128 -r0 -m1 -v > somegame.header\n\n");
    printf("Options:\n");
    printf("\t-b : Battery Backed\n");
    printf("\t-c[value] : CHR-ROM size (kb)\n");
    printf("\t-f : Four-Screen\n");

}

int main (int argc, char *argv[]) {
	const char *FLAG_FORMAT = "bc:fhm:p:r:tv";

    int prgrom_kb = 16;
    int chrrom_kb = 8;
    int prgram_kb = 0;
    int mapper = 0;
    int four_screen = 0;
    int trainer_present = 0;
    int battery_backed = 0;
    int vertical_mirror = 1;

    //TODO: Add -o file output.
    int rv = 0;
	while((rv = getopt(argc, argv, FLAG_FORMAT)) != -1) {
        switch(rv) {
            case 'b':
                battery_backed = 1;
                break;
            case 'c':
                chrrom_kb = atoi(optarg);
                break;
            case 'f':
                four_screen = 1;
                break;
            case 'h':
                help();
                return 0;
            case 'm':
                mapper = atoi(optarg);
                break;
            case 'p':
                prgrom_kb = atoi(optarg);
                break;
            case 'r':
                prgram_kb = atoi(optarg);
                break;
            case 't':
                trainer_present = 1;
                break;
            case 'v':
                vertical_mirror = 1;
                break;
            default:
				return 1;
        }
    }

    // TODO: Sanity check values.

    writeHeader(prgrom_kb, chrrom_kb, prgram_kb, mapper, four_screen, trainer_present, battery_backed, vertical_mirror);

    return 0;
}
