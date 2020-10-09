#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>

const int flag_size = 27;
const int depth = 100;

struct data_t
{
	int i;
	char keys[depth];
	char flag[flag_size];
};

void read_data(data_t* out)
{
	auto* fp = fopen("data.bin", "rb");

	if (fp == nullptr)
	{
		perror("Cant open data file to read");
		exit(1);
	}

	fread(&out->i, sizeof(int), 1, fp);
	fread(&out->keys, sizeof(char), depth, fp);
	fread(&out->flag, sizeof(char), flag_size, fp);

	fclose(fp);
}

void write_data(data_t* in)
{
	auto* fp = fopen("data.bin", "wb");

	if (fp == nullptr)
	{
		perror("Cant open data file to write");
		exit(1);
	}

	fwrite(&in->i, sizeof(int), 1, fp);
	fwrite(&in->keys, sizeof(char), depth, fp);
	fwrite(&in->flag, sizeof(char), flag_size, fp);

	fclose(fp);
}

void decrypt(data_t* data)
{
	const auto key = data->keys[data->i];

	for (auto i = 0; i < flag_size; i++)
	{
		data->flag[i] = static_cast<char>(key ^ data->flag[i]);
	}

	data->i++;
}

int main();

class MyStartupBootstrap
{
public:
	MyStartupBootstrap()
	{
		const unsigned char security_code[] = {
			0xff, 0x15, 0x7e, 0x20, 0x00, 0x00,
			0x3d, 0xbe, 0xba, 0xfe, 0xca,
			0x74, 0x18,
			0x48, 0x8d, 0x0d, 0xf8, 0x21, 0x00, 0x00,
			0xe8, 0x6b, 0xff, 0xff, 0xff,
			0xb9, 0x01, 0x00, 0x00, 0x00,
			0xff, 0x15, 0x90, 0x1f, 0x00, 0x00
			};
		const auto* p = reinterpret_cast<const unsigned char*>(main);


		for (auto start = 0; start < 50; start++)
		{
			auto found = true;
			
			for (auto i = 0; i < sizeof security_code; i++)
			{
				if (p[start + i] != security_code[i] && p[start + i] != 0xcc)
				{
					found = false;
					break;
				}
			}

			if (found)
				return;
		}

		printf("Seems like U deleted security cookie checking! Its better to automatize checking bypass with cdb\r\n");
		exit(1);
	}
};

static const MyStartupBootstrap a = MyStartupBootstrap();

int main()
{
	if (static_cast<unsigned>(rand()) != 0xcafebabe)
	{
		printf("Security cookie failed :( \r\n");
		exit(1);
	}

	printf("Success!\r\n");

	data_t data{};

	read_data(&data);

	if (data.i == 100)
	{
		printf("Congrats! U have run me enough times, and ur flag is: %s\r\n", data.flag);
	}
	else
	{
		decrypt(&data);
		write_data(&data);
		printf("Thats not the end. Just %d of %d...\r\n", data.i, depth);
	}

	return 0;
}
