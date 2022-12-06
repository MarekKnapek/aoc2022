#define _CRT_SECURE_NO_WARNINGS


#include <stddef.h>
#include <assert.h>
#include <stdio.h>


int mk_aoc_find(unsigned char const needle, unsigned char const* const haystack, int const len, int* const position)
{
	int ret;
	int i;

	assert(haystack);
	assert(len >= 0);
	assert(position);

	ret = len;
	for(i = 0; i != len; ++i)
	{
		if(haystack[i] == needle)
		{
			ret = i;
			break;
		}
	}
	*position = ret;
	return 0;
}

int mk_aoc_parse_int_line(FILE* const file, int* const eof, int* const empty, int* const num)
{
	static char const s_digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	size_t read;
	unsigned char elem;
	int err;
	int digit;
	int ret;

	assert(sizeof(s_digits) / sizeof(s_digits[0]) == 10);
	assert(file);
	assert(eof);
	assert(empty);
	assert(num);

	if(ferror(file) != 0)
	{
		return ((int)(__LINE__));
	}
	if(feof(file) != 0)
	{
		*eof = 1;
		return 0;
	}
	read = fread(&elem, 1, 1, file);
	if(read != 1)
	{
		if(ferror(file) != 0)
		{
			return ((int)(__LINE__));
		}
		if(feof(file) != 0)
		{
			*eof = 1;
			return 0;
		}
		return ((int)(__LINE__));
	}
	if(elem == 0x0a || elem == 0x0d)
	{
		*eof = 0;
		*empty = 1;
		return 0;
	}
	err = mk_aoc_find(elem, s_digits, 10, &digit);
	if(err != 0) return err;
	if(digit == 10) return ((int)(__LINE__));
	ret = digit;
	for(;;)
	{
		read = fread(&elem, 1, 1, file);
		if(read != 1)
		{
			if(ferror(file) != 0)
			{
				return ((int)(__LINE__));
			}
			if(feof(file) != 0)
			{
				break;
			}
			return ((int)(__LINE__));
		}
		if(elem == 0x0a || elem == 0x0d)
		{
			break;
		}
		err = mk_aoc_find(elem, s_digits, 10, &digit);
		if(err != 0) return err;
		if(digit == 10) return ((int)(__LINE__));
		ret = ret * 10 + digit;
	}
	*eof = 0;
	*empty = 0;
	*num = ret;
	return 0;
}

int mk_aoc2022_day01(char const* const input, int* const out)
{
	FILE* f;
	int err;
	int eof;
	int empty;
	int calories;
	int biggest;
	int current;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	/* first line of first elf */
	err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
	if(err != 0) return err;
	if(eof != 0) return ((int)(__LINE__));
	if(empty != 0) return ((int)(__LINE__));
	biggest = calories;
	for(;;)
	{
		/* rest lines of first elf */
		err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
		if(err != 0) return err;
		if(eof != 0) goto break2;
		if(empty != 0) break;
		biggest += calories;
	}
	for(;;)
	{
		/* first line of rest elf */
		err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
		if(err != 0) return err;
		if(eof != 0) goto break2;
		if(empty != 0) break;
		current = calories;
		for(;;)
		{
			/* rest lines of rest elf */
			err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
			if(err != 0) return err;
			if(eof != 0) goto break2;
			if(empty != 0) break;
			current += calories;
		}
		if(current > biggest) biggest = current;
	}
	break2:;
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	*out = biggest;
	return 0;
}

int mk_aoc2022_day01b(char const* const input, int* const out)
{
	int biggest[3];
	FILE* f;
	int err;
	int eof;
	int empty;
	int calories;
	int current;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	biggest[1] = 0;
	biggest[2] = 0;
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	/* first line of first elf */
	err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
	if(err != 0) return err;
	if(eof != 0) return ((int)(__LINE__));
	if(empty != 0) return ((int)(__LINE__));
	biggest[0] = calories;
	for(;;)
	{
		/* rest lines of first elf */
		err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
		if(err != 0) return err;
		if(eof != 0) goto break2;
		if(empty != 0) break;
		biggest[0] += calories;
	}
	for(;;)
	{
		/* first line of rest elf */
		err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
		if(err != 0) return err;
		if(eof != 0) goto break2;
		if(empty != 0) break;
		current = calories;
		for(;;)
		{
			/* rest lines of rest elf */
			err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
			if(err != 0) return err;
			if(eof != 0) goto break2;
			if(empty != 0) break;
			current += calories;
		}
		if(current > biggest[0])
		{
			biggest[2] = biggest[1];
			biggest[1] = biggest[0];
			biggest[0] = current;
		}
		else if(current > biggest[1])
		{
			biggest[2] = biggest[1];
			biggest[1] = current;
		}
		else if(current > biggest[2])
		{
			biggest[2] = current;
		}
	}
	break2:;
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	*out = biggest[0] + biggest[1] + biggest[2];
	return 0;
}


#include <stdio.h>

int main(void)
{
	int err;
	int ret;

	printf("%s\n", "Day 1");
	err = mk_aoc2022_day01("input01a.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day01("input01b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day01b("input01b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	return 0;
}
