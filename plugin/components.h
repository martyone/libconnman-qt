/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <QtPlugin>

#include <QDeclarativeExtensionPlugin>

class Components : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void registerTypes(const char *uri);

    void initializeEngine(QDeclarativeEngine *engine, const char *uri);
};

#endif // COMPONENTS_H
