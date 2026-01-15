#ifndef SCRIPTSCONSOLE_H
#define SCRIPTSCONSOLE_H
#include <QObject>
#include <lua.hpp>

class ScriptsConsole : public QObject {
    Q_OBJECT
public:
    explicit ScriptsConsole(QObject* parent = nullptr);
    ~ScriptsConsole();

    void init();
    void shutdown();
    void execute(const QString& code);

signals:
    void output(const QString& text);

private:
    static int luaPrintRedirect(lua_State* L);
    void installPrint();
    void registerBindings();
    void globalizeProto();
    void emitStackResults(int baseTop);

    lua_State* L = nullptr;
};

#endif // SCRIPTSCONSOLE_H
