int digit_cnt(char *c){
    int length = 0;
    char curr;
    for (int i = 0; (curr = c[i]) != 0; i++){
        if (curr >= '0' && curr <= '9')
            length++;
    }
    return length;
}

int main(int argc, char **argv){
    char *c;
    c = argv[1];
    digit_cnt(c);
    return 0;
}