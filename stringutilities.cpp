#include "stringutilities.h"

QString withFileExtension(QString path, QString extension){
    return path.left(path.lastIndexOf('.')) + "." + extension;
}
