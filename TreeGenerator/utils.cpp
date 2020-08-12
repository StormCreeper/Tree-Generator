#include "utils.h"

unsigned int createShader(unsigned int shaderType, const char* shaderSource) {
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

    int success;
    char infoLog[2048];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 2048, NULL, infoLog);
        std::cerr << "Shader compilation error :\n" << infoLog << std::endl;
        system("PAUSE");
        exit(-1);
    }

	return shader;
}

unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader) {
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

    return shaderProgram;
}

unsigned int createProgram(const char* vname, const char* fname) {
    char* vertSource = readSource(vname);
    char* fragSource = readSource(fname);

    unsigned int shader = createProgram(createShader(GL_VERTEX_SHADER, vertSource), createShader(GL_FRAGMENT_SHADER, fragSource));

    free(vertSource);
    free(fragSource);

    return shader;
}

char* readSource(const char* filename) {
    char* text = NULL;

    if (filename != NULL) {
        FILE* file;
        fopen_s(&file, filename, "rt");

        if (file != NULL) {
            fseek(file, 0, SEEK_END);
            int count = ftell(file);
            rewind(file);

            if (count > 0) {
                text = (char*)malloc(sizeof(char) * (count + 1));
                count = fread(text, sizeof(char), count, file);
                text[count] = '\0';
            }
            fclose(file);
        }
    }
    else {
        std::cerr << "Nom de fichier invalide";
    }
    return text;
}

void setUniformM4(unsigned int shader, const char* name, glm::mat4 matrix) {
    unsigned int loc = glGetUniformLocation(shader, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}
void setUniformVec3(unsigned int shader, const char* name, glm::vec3 vector) {
    unsigned int loc = glGetUniformLocation(shader, name);
    glUniform3f(loc, vector.x, vector.y, vector.z);
}
void setUniformInt(unsigned int shader, const char* name, int value) {
    unsigned int loc = glGetUniformLocation(shader, name);
    glUniform1i(loc, value);
}
void setUniformFloat(unsigned int shader, const char* name, float value) {
    unsigned int loc = glGetUniformLocation(shader, name);
    glUniform1f(loc, value);
}

float lerp(float a, float b, float c) {
    return a + c*(b-a);
}