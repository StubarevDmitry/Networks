#include "session.hpp"



void createUploadsFolder() {
    std::filesystem::path uploadsPath("uploads");

    // Проверяем, существует ли папка
    if (!std::filesystem::exists(uploadsPath)) {
        std::filesystem::create_directory(uploadsPath);
        std::cout << "Папка 'uploads' успешно создана!" << std::endl;
    }
}

std::string readUntilMinus(tcp::socket& socket) {
    std::string received_data;
    char c;

    while (true) {
        // Считываем один байт из сокета
        boost::system::error_code error;
        size_t bytes_read = socket.read_some(boost::asio::buffer(&c, 1), error);

        if (error) {
            std::cerr << "Error reading from socket: " << error.message() << std::endl;
            return " "; // возвращаем " "
        }

        // Добавляем считанный символ в строку
        received_data += c;

        if (c == '-') {
            break;
        }
    }

    return received_data;
}

std::string getUniqueFileName(std::string baseName) {
    std::string newName = baseName;
    int copyNumber = 1;

    // Проверяем, существует ли файл, если да - генерируем новое имя
    while (std::filesystem::exists("uploads\\" + newName)) {

        // Извлекаем имя файла и его расширение
        std::string::size_type dotPos = baseName.find_last_of('.');
        std::string nameWithoutExt = (dotPos == std::string::npos) ? baseName : baseName.substr(0, dotPos);
        std::string extension = (dotPos == std::string::npos) ? "" : baseName.substr(dotPos);

        // Генерируем имя нового файла
        newName = nameWithoutExt + "(" + std::to_string(copyNumber) + ")" + extension;
        copyNumber++;
    }

    return newName;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens; // Вектор для сохранения слов

    std::string token; // Переменная для хранения текущего слова

    std::istringstream tokenStream(str); // Использование stringstream для обработки строки

    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

Session::Session(tcp::socket socket) : socket_(std::move(socket)) {}

void Session::start() {
    readFileNameAndSize();
}


void Session::readFileNameAndSize() {

    std::string file_metadata = readUntilMinus(socket_);

    file_metadata.pop_back();

    std::vector<std::string> words = split(file_metadata, ' ');

    std::cout << "Прочитал имя файла: " << words[1] << std::endl;
    fileName = words[1];

    fileSize = std::atoi(words[0].data());
    std::cout << "Прочитал размер файла: " << fileSize << std::endl;

    readFile();
}

void Session::readFile() {

    createUploadsFolder();

    std::ofstream ofs(uploads + getUniqueFileName(fileName), std::ios::binary);

    SpeedWriter test;
    test.timerRun();
    std::size_t total_read = 0;

    while (total_read != fileSize) {
        std::size_t lengthToRead = socket_.read_some(boost::asio::buffer(data, size_t(bufferSize)));

        //запись в файл
        ofs.write(data, lengthToRead);

        total_read += lengthToRead;

        test.addBytes(lengthToRead);

        test.timeCheckAndUpdateTime();
    }

    test.writeSpeedAndUpdateTime();

    if (!ofs) {
        std::cout << "не удачно с копировано " << std::endl;
        doWrite("ERROR");
    }
    else {
        std::cout << "Удачно с копировано " << std::endl;
        doWrite("all good");
    }

    ofs.close();
    socket_.close();
}


void Session::doWrite(std::string massage) {
    boost::asio::write(socket_, boost::asio::buffer(massage));
}