#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int error(FILE *file, const char *message) {
	perror(message);
	fclose(file);
	return 1;
}

const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/** Detect the file type of image
 * Supported types: .jpeg .jpg .png .gif .bmp .webp */
const char *detect(const unsigned char *buf, size_t length) {
	if (length < 4) {
		perror("Bro, this ain't image.");
		return NULL;
	}
	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF)
		return "jpeg";
	else if (buf[0] == 0x89 && buf[1] == 0x50 && buf[2] == 0x4E && buf[3] == 0x47 &&
			buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A)
		return "png";
	else if (buf[0] == 0x47 && buf[1] == 0x49 && buf[2] == 0x46 && buf[3] == 0x38)
		return "gif";
	else if (buf[0] == 0x42 && buf[1] == 0x4D)
		return "bmp";
	else if (buf[0] == 0x52 && buf[1] == 0x49 && buf[2] == 0x46 && buf[3] == 0x46 &&
			buf[8] == 0x57 && buf[9] == 0x45 && buf[10] == 0x42 && buf[11] == 0x50)
		return "webp";
	else
		return NULL;
}

/* Transform image data to base64 code */
int encode(const unsigned char *in, size_t length_in) {
	/**
	 * ====== Base64 Encoding Algorithm =====
	 * 	1. @in Each element contains 1 bytes;
	 * 	2. @length_in The array length of @in;
	 * 	3. @length_out The array length of encoded sequence;
	 * 		Every 6 bits is mapped to 1 Base64 character,
	 * 		every 3 bytes is mapped to 4 Base64 character.
	 **/

	size_t length_out = ((length_in + 2) / 3) * 4;
	char *out = (char *) malloc(length_out + 1);
	size_t i, j;

	if (out == NULL) {
		perror("Failed at allocating memory.");
		return 1;
	}
	for (i = 0, j = 0; i < length_in;) {
		/* @a @b @c are octets (8-bits tuple). 3 bytes => 4 output characters. */
		unsigned int a = i < length_in ? in[i++] : 0;
		unsigned int b = i < length_in ? in[i++] : 0;
		unsigned int c = i < length_in ? in[i++] : 0;

		/* If a = 0x41, b = 0x42, c = 0x43, str = 0x414243. @str contains 24 bits. */
		unsigned int str = (a << 16) | (b << 8) | c;
		out[j++] = table[(str >> 18) & 0x3F];
		out[j++] = table[(str >> 12) & 0x3F];
		out[j++] = (i > length_in + 1) ? '=' : table[(str >> 6) & 0x3F];
		out[j++] = (i > length_in) ? '=' : table[str & 0x3F];
	}
	out[length_out] = '\0';
	printf("![](data:image/%s;base64,%s)\n", detect(in, length_in), out);
	return 0;
}

int main(int argc, const char *argv[]) {
	freopen("output.md", "w", stdout);
	int f;

	if (argc < 2)
		return error(NULL, "Drag image files to start.");
	for (f = 1; f < argc; ++f) {
		const char *fp = argv[f];
		FILE *file = fopen(fp, "rb");
		size_t result, size;
		static unsigned char *buf;

		/* Get the size of the file */
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		rewind(file);

		/* Allocate memory */
		buf = (unsigned char *) malloc(size);
		if (buf == NULL)
			return error(file, "Failed allocating memory.");


		/* Read binary data into @buf */
		result = fread(buf, 1, size, file);
		if (result != size)
			return error(file, "Error reading file.");

		encode(buf, size);
		free(buf);
		fclose(file);
	}
	system("code output.md");
	getchar();
	system("rm output.md");
}
