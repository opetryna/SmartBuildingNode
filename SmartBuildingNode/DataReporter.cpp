#include <stdlib.h>

#include "DataReporter.h"

DataReporter::DataReporter(void (*send) (char*, unsigned int), bool autoSubmit) {
    this->autoSubmit = autoSubmit;
    this->send = send;
    this->init();
}

void DataReporter::init() {
    this->p = 0;
    this->msg[p++] = '{';
    this->msg[p++] = '}';
    this->msg[p] = '\0';
}

void DataReporter::countChars(char* s, struct CharsCount* c) {
    while ('\0' != *s) {
        if ('\b' == *s)
            c->special++;
        else if ('\f' == *s)
            c->special++;
        else if ('\n' == *s)
            c->special++;
        else if ('\r' == *s)
            c->special++;
        else if ('\t' == *s)
            c->special++;
        else if ('"' == *s)
            c->special++;
        else if ('\\' == *s)
            c->special++;
        c->total++;
        s++;
    }
}

void DataReporter::formatString(char* dst, char* src, bool escape, bool quoted) {
    if (quoted)
        *(dst++) = '"';
    if (escape) {
        while ('\0' != *src) {
            if ('\b' == *src) {
                *(dst++) = '\\'; *(dst++) = 'b';
            } else if ('\f' == *src) {
                *(dst++) = '\\'; *(dst++) = 'f';
            } else if ('\n' == *src) {
                *(dst++) = '\\'; *(dst++) = 'n';
            } else if ('\r' == *src) {
                *(dst++) = '\\'; *(dst++) = 'r';
            } else if ('\t' == *src) {
                *(dst++) = '\\'; *(dst++) = 't';
            } else if ('"' == *src) {
                *(dst++) = '\\'; *(dst++) = '"';
            } else if ('\\' == *src) {
                *(dst++) = '\\'; *(dst++) = '\\';
            } else {
                *(dst++) = *src;
            }
            src++;
        }
    } else {
        while ('\0' != *src)
            *(dst++) = *(src++);
    }
    if (quoted)
        *(dst++) = '"';
}

bool DataReporter::add(char* key, char* value, bool isString) {

    struct CharsCount cKey; countChars(key, &cKey);
    unsigned int keySize = cKey.total + cKey.special + 2;
    struct CharsCount cValue; countChars(value, &cValue);
    unsigned int valueSize = cValue.total;
    if (isString)
        valueSize += cValue.special + 2;

    unsigned int size = keySize + valueSize + 3;
    if (p != 2) size++;

    if (p - 1 + size > MAX_MSG_SIZE) {
        if (autoSubmit)
            submit();
        else
            return false;
        if (p - 1 + size > MAX_MSG_SIZE)
            return false;
    }

    if (p != 2)
        msg[p - 1] = ',';
    else
        p--;
    formatString(msg + p, key, cKey.special != 0, true);
    p += keySize;
    msg[p++] = ':';
    formatString(msg + p, value, isString && cValue.special != 0, isString);
    p += valueSize;
    msg[p++] = '}';
    msg[p] = '\0';

    return true;
}

bool DataReporter::add(char* key, char* value) {
    return add(key, value, true);
}

bool DataReporter::add(char* key, int value) {
    char value_s[4];
    itoa(value, value_s, 10);
    return add(key, value_s, false);
}

bool DataReporter::add(char* key, float value) {
    char value_s[6];
    dtostrf(value, 5, 1, value_s);
    char *value_s_p = value_s;
    while (' ' == *value_s_p) value_s_p++;
    return add(key, value_s_p, false);
}

void DataReporter::submit() {
    send(msg, p + 1);
    init();
}