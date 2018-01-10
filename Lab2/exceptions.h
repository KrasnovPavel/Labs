#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include <vector>

/**
 * @brief The FileOpeningFailure class
 */
class FileOpeningFailure : public std::exception
{
public:
    explicit FileOpeningFailure(const std::string& filename)
    {
        m_str = "Unable to open file: " + filename;
    }

    const char *what() const noexcept override
    {
        return m_str.c_str();
    }

private:
    std::string m_str;
};


/**
 * @brief The FileFormatError class
 *
 * @details This exception throws if file format isn't supported.
 */
class FileFormatError : public std::exception
{
public:
    /**
     * Constructor
     * @param filename
     * @param supportedFormats
     */
    FileFormatError(const std::string& filename, const std::vector<std::string>& supportedFormats)
    {
        m_str = "\nFormat of: \"" + filename + "\" is not supported. \nSupported formats:\n";
        for (auto s: supportedFormats)
        {
            m_str += "    " + s + "\n";
        }
    }

    const char *what() const noexcept override
    {
        return m_str.c_str();
    }

private:
    std::string m_str;
};


/**
 * @brief The FileParcingError class
 *
 * @details This exception throws if program can't parse file.
 */
class FileParsingError : public std::exception
{
public:
    /**
     * Constructor
     * @param filename
     * @param errorName
     */
    FileParsingError(const std::string& filename, const std::string& errorName = "")
    {
        m_str = "Error" + (!errorName.empty()?" \""+errorName+"\"":"") + " occurred while parsing: " + filename;
    }

    const char *what() const noexcept override
    {
        return m_str.c_str();
    }

private:
    std::string m_str;
};
#endif // EXCEPTIONS_H
