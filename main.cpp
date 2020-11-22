#include <string>

template <int N>
constexpr auto count_width(const char (&fmt)[N])
{
    int counter = 0;
    for(int i = 0 ; fmt[i] != '\0'; i++)
    {
        if(fmt[i] == '%' && fmt[++i] != '%')
        {
            counter++;
        }
    }
    return counter*9 + N;
}


constexpr bool is_specifier(char c)
{
    char spec[] = "csdioxXufFeEaAgGnp";
    for(int i = 0; i < sizeof(spec) / sizeof(spec[0]); i++)
        if(c == spec[i])
            return true;
    return false;
}


template<int DATA_SIZE, int FMT_SIZE>
constexpr auto add_color(const char (&fmt)[FMT_SIZE])
{
    union data_t{char value[DATA_SIZE] = {'\0'};};
    data_t data;
    int data_pos = 0;
    for(int i = 0 ; i < FMT_SIZE; i++)
    {
        if(fmt[i] == '%')
        {
            if(fmt[i+1] == '%')
            {
                data.value[i + data_pos] = '%';
                data.value[++i + data_pos] = '%';
            }
            else
            {
                int color_pos = 0;
                data.value[i + data_pos] = '\e';
                data.value[i + ++data_pos] = '[';
                data.value[i + ++data_pos] = '9';
                data.value[color_pos = (i + ++data_pos)] = '0';
                data.value[i + ++data_pos] = 'm';
                data.value[i + ++data_pos] = fmt[i];
                while( !is_specifier(fmt[++i]))
                {
                    data.value[i + data_pos] = fmt[i];
                }
                data.value[color_pos] = (fmt[i] == 's') ? '5' : '2';
                data.value[i + data_pos] = fmt[i];
                data.value[i + ++data_pos] = '\e';
                data.value[i + ++data_pos] = '[';
                data.value[i + ++data_pos] = '0';
                data.value[i + ++data_pos] = 'm';
            }
        }
        else
        {
            data.value[i + data_pos] = fmt[i];
        }
    }
    return data;
}



#define COLOR_FMT(fmt) []{ \
     static constexpr auto data = add_color<count_width(fmt)>(fmt); \
     return data.value;}()


#define STR "[ %-12.12s] = [%d]\n"

int main()
{
    printf(COLOR_FMT(STR), "hello world", 10);

    return 0;
}
