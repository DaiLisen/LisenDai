#include <stdio.h>
#include <string.h>
/**
 * Parses pattern for next character and sets
 * necessary parameters that are referenced inside/outside
 * of this function.
 *
 * Add any additional inputs to function signature if necessary.
 */

char parse_char(char *partial_pattern, char *target) {
    // You can use this recommended helper function, or not.
    switch (*partial_pattern) {
        case '.':
            return *target;
            break;
        default:
            return *partial_pattern;
    }
}
/**
 * Returns true if partial_line matches pattern, starting from
 * the first char of partial_line.
 */
int matches_leading(char *partial_line, char *pattern) {
    char *p = pattern, *q = partial_line, temp;
    while(*partial_line != '\0'){
        if(*p == '\\'){
            if(*partial_line == *++p){
                p++;
            }
            else{
                p = pattern;
                if(*partial_line == parse_char(pattern, partial_line))
                    partial_line--;
            }
        }
        else if(*(p+1) == '?'){
            if(*p == '.'){
                if(*(partial_line+1) == *(p+2)){
                    p++;
                    p++;
                }
                else{
                    p++;
                    p++;
                    partial_line--;
                }
            }
            else if(*partial_line == *p){
                if(*(p+2) == *partial_line)
                    partial_line--;
                p++;p++;
            }
            else{
                p++;p++;
                if(*p != *partial_line);
                else
                    partial_line--;
            }
        }
        else if(*p == '+')
        {
            temp = parse_char((p-1), (partial_line-1));
            p++;
            while(*p == *partial_line){
                p++;
            }
            while(*partial_line == temp){
                partial_line++;
            }
            partial_line--;
        }
        else if(*p == '('){
            while(*q != '\0'){
                if(*q == '(' || *q == ')')
                    break;
                q++;
            }
            if(*q == 0)
                return 0;
            else{
                q = partial_line;
                p++;
                continue;
            }
        }
        else if(*partial_line == parse_char(p, partial_line)){
            p++;
        }
        else{
            p = pattern;
            if(*partial_line == parse_char(pattern, partial_line))
                partial_line--;
        }
        if(*p == '\0')
            return 1;
        partial_line ++;
        if(*partial_line == '\0' && (*p == '+' || *p == '?'))
            return 1;
    }
    return 0;
}

/**
 * Implementation of your matcher function, which
 * will be called by the main program.
 *
 * You may assume that both line and pattern point
 * to reasonably short, null-terminated strings.
 *
 * This function is REQUIRED and is not a suggestion.
 */
int rgrep_matches(char *line, char *pattern) {
    return matches_leading(line, pattern);
    //
    // TODO: put your code here.
    //
    
    return 0;
}






int main() {
    char a[100],b[100];
    printf("Enter string you want to search:\n");
    gets(a);
    printf("THEN enter the pattern you search for:\n");
    gets(b);
    switch (rgrep_matches(a, b)) {
        case 0:
            printf("No such a pattern\n");
            break;
        case 1:
            printf("Yes! We got it!\n");
            break;
            
        default:
            break;
    }
    return 0;
}

