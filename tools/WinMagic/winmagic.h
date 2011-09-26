#ifndef WINTEST_H
#define WINTEST_H

#include <QtGui/QWidget>
#include "ui_winmagic.h"
#include "render.h"

struct BlendFuncFactor{
    QString str;
    int code;
};
static const BlendFuncFactor BlendFuncFactors [] = {
    {"GL_ZERO",	                     GL_ZERO                    },
    {"GL_ONE",	                     GL_ONE                     },
    {"GL_SRC_COLOR",	             GL_SRC_COLOR               },
    {"GL_ONE_MINUS_SRC_COLOR",	     GL_ONE_MINUS_SRC_COLOR     },
    {"GL_DST_COLOR",	             GL_DST_COLOR               },
    {"GL_ONE_MINUS_DST_COLOR",	     GL_ONE_MINUS_DST_COLOR     },
    {"GL_SRC_ALPHA",	             GL_SRC_ALPHA               },
    {"GL_ONE_MINUS_SRC_ALPHA",	     GL_ONE_MINUS_SRC_ALPHA     },
    {"GL_DST_ALPHA",	             GL_DST_ALPHA               },
    {"GL_ONE_MINUS_DST_ALPHA",	     GL_ONE_MINUS_DST_ALPHA     },
    {"GL_CONSTANT_COLOR",	         GL_CONSTANT_COLOR          },
    {"GL_ONE_MINUS_CONSTANT_COLOR",	 GL_ONE_MINUS_CONSTANT_COLOR},
    {"GL_CONSTANT_ALPHA",	         GL_CONSTANT_ALPHA          },
    {"GL_ONE_MINUS_CONSTANT_ALPHA",	 GL_ONE_MINUS_CONSTANT_ALPHA},
    {"GL_SRC_ALPHA_SATURATE",        GL_SRC_ALPHA_SATURATE      },
    {"END", 0}
};


class WinMagic : public QWidget
{
    Q_OBJECT
    Render m_render;
public:
    WinMagic(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~WinMagic();

    static int getGLBlendFuncFromStr(const QString & codeStr);

private:
    Ui::WinTestClass ui;
    void setupTest();
    void setupResEditer();

public slots:
    void on_cb_engine_currentIndexChanged(int index);
    void on_clb_save_clicked();
    void on_clb_selectImg_clicked();
    void on_clb_restore_clicked();
    void on_cmb_covers_currentIndexChanged ( const QString & text );
    void on_cmb_frames_currentIndexChanged ( const QString & text );
    void on_cmb_effect_currentIndexChanged ( const QString & text );

    void on_tabWidget_currentChanged(int index);

    void on_cmb_preset_currentIndexChanged(int index);
    void on_btn_preview_clicked();
    void on_btn_saveRes_clicked();
    void on_btn_selResImg_clicked();

};

#endif // WINTEST_H
