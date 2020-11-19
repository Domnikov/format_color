#include <array>
#include <iostream>

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


template<int N>
struct add_color
{
    constexpr add_color(const char* fmt)
        :m_data{'\0'}
    {
        int data_pos = 0;
        for(int i = 0 ; fmt[i] != '\0'; i++)
        {
            if(fmt[i] == '%')
            {
                if(fmt[i+1] == '%')
                {
                    m_data[i + data_pos] = '%';
                    m_data[++i + data_pos] = '%';
                }
                else
                {
                    int color_pos = 0;
                    m_data[i + data_pos] = '\e';
                    m_data[i + ++data_pos] = '[';
                    m_data[i + ++data_pos] = '9';
                    m_data[color_pos = (i + ++data_pos)] = '0';
                    m_data[i + ++data_pos] = 'm';
                    m_data[i + ++data_pos] = fmt[i];
                    while( !is_specifier(fmt[++i]))
                    {
                        m_data[i + data_pos] = fmt[i];
                    }
                    m_data[color_pos] = (fmt[i] == 's') ? '5' : '2';;
                    m_data[i + data_pos] = fmt[i];
                    m_data[i + ++data_pos] = '\e';
                    m_data[i + ++data_pos] = '[';
                    m_data[i + ++data_pos] = '0';
                    m_data[i + ++data_pos] = 'm';
                }
            }
            else
            {
                m_data[i + data_pos] = fmt[i];
            }
        }
    }

    constexpr bool is_specifier(char c)
    {
        char spec[] = "csdioxXufFeEaAgGnp";
        for(int i = 0; i < sizeof(spec) / sizeof(spec[0]); i++)
            if(c == spec[i])
                return true;
        return false;
    }

    const char* data()const noexcept{return m_data;}


private:
    char m_data[N];
};

#define STR "[%10.10s] = [%d]\n"

#define color_print(fmt, ...) \
    printf(add_color< count_width(fmt)>(fmt).data(), ##__VA_ARGS__)

int main()
{
    constexpr auto fmt = add_color< count_width(STR) >(STR);
    std::cout << fmt.data() << std::endl;

    color_print(STR, "helow world", 10);
    return 0;
}
