#ifndef QUALITY_COLOR_H
#define QUALITY_COLOR_H

#include <QString>

static inline QString getQualityColor(int quality)
{
    QString color;

    if (quality == 100)
        color = "#FE9600";
    else if (quality >= 90)
        color = "#CE43FC";
    else if (quality >= 70)
        color = "#00B5FF";
    else if (quality >= 30)
        color = "#00B700";
    else if (quality >= 10)
        color = "#FFD200";
    else
        color = "#FF6000";

    return color;
}

#endif // QUALITY_COLOR_H
