int flag_to_base(char format) {
/*
    Returns base of formats d, x, o, b 
*/

    switch (format) {
        case 'x':
            return 16;
        case 'd':
            return 10;
        case 'o':
            return 8;
        case 'b':
            return 2;
    }

    //For security, but shouldnt be triggered.
    return 10;
}