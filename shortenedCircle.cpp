void drawCircle(Color image[][WIDTH], int height, int centerX, int centerY, int radius, const Color color) {
    if (centerX > WIDTH || centerX < 0 || centerY > HEIGHT || centerY < 0 || centerY + radius > HEIGHT || centerY - radius < 0 || centerX + radius > WIDTH || centerX - radius < 0) //bounds checking
        return;

    for (int x = radius * -1 ; x <= radius; x++) {
        for (int row = (int) sqrt((radius * radius) - (x * x)) + centerY; row >= centerY - (int) sqrt((radius * radius) - (x * x)); row--) { //column-order (slower)
            image[row][x + centerX] = color;
        }
    }

}
