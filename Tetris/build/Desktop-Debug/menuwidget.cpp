#include "menuwidget.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QLinearGradient>

MenuWidget::MenuWidget(QWidget *parent) : QWidget(parent)
{
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Введите никнейм...");
    nameEdit->setMaxLength(16);
    nameEdit->setAlignment(Qt::AlignCenter);
    nameEdit->setStyleSheet(R"(
        QLineEdit {
            background: rgba(255,255,255,15);
            border: 2px solid rgba(100,100,200,180);
            border-radius: 8px;
            color: white;
            font-size: 16px;
            padding: 6px 12px;
        }
        QLineEdit:focus {
            border-color: rgba(150,150,255,230);
        }
    )");

    startBtn = new QPushButton("▶  ИГРАТЬ", this);
    startBtn->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #5555cc, stop:1 #333399);
            border: 2px solid #7777ee;
            border-radius: 10px;
            color: white;
            font-size: 18px;
            font-weight: bold;
            padding: 10px 30px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #7777ee, stop:1 #5555cc);
        }
        QPushButton:pressed {
            background: #333399;
        }
    )");

    scoresLabel = new QLabel(this);
    scoresLabel->setAlignment(Qt::AlignCenter);
    scoresLabel->setStyleSheet("color: rgba(200,200,255,200); font-size: 13px;");
    scoresLabel->setText("Рекордов пока нет");

    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setAlignment(Qt::AlignCenter);
    lay->setSpacing(18);
    lay->addStretch(3);
    lay->addWidget(nameEdit,    0, Qt::AlignCenter);
    lay->addWidget(startBtn,    0, Qt::AlignCenter);
    lay->addSpacing(24);
    lay->addWidget(scoresLabel, 0, Qt::AlignCenter);
    lay->addStretch(2);

    nameEdit->setFixedWidth(220);
    startBtn->setFixedWidth(220);

    connect(startBtn, &QPushButton::clicked, this, [this]() {
        QString name = nameEdit->text().trimmed();
        if (name.isEmpty()) name = "Игрок";
        emit startRequested(name);
    });
    connect(nameEdit, &QLineEdit::returnPressed, startBtn, &QPushButton::click);
}

void MenuWidget::refreshScores(const QVector<ScoreEntry> &scores)
{
    if (scores.isEmpty()) {
        scoresLabel->setText("Рекордов пока нет");
        return;
    }
    QString text = "<b style='color:#aaaaff;font-size:14px;'>🏆 РЕКОРДЫ</b><br><br>";
    const QStringList medals = {"🥇","🥈","🥉"};
    for (int i = 0; i < scores.size(); ++i) {
        QString medal = (i < 3) ? medals[i] : QString::number(i+1)+".";
        text += QString("%1 &nbsp;<b>%2</b>&nbsp;&nbsp;%3<br>")
                    .arg(medal)
                    .arg(scores[i].name.toHtmlEscaped())
                    .arg(scores[i].score);
    }
    scoresLabel->setText(text);
}

void MenuWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QLinearGradient bg(0, 0, 0, height());
    bg.setColorAt(0.0, QColor(15, 15, 30));
    bg.setColorAt(1.0, QColor(5,   5, 15));
    p.fillRect(rect(), bg);

    // Заголовок
    p.setFont(QFont("Arial", 52, QFont::Bold));
    QLinearGradient tg(0, height()*0.10, 0, height()*0.32);
    tg.setColorAt(0.0, QColor(150, 150, 255));
    tg.setColorAt(0.5, QColor(80,   80, 220));
    tg.setColorAt(1.0, QColor(40,   40, 160));
    p.setPen(QPen(QBrush(tg), 1));
    p.drawText(QRect(0, 0, width(), (int)(height()*0.40)),
               Qt::AlignHCenter | Qt::AlignBottom, "TETRIS");

    // Разделитель
    p.setPen(QColor(80, 80, 160));
    p.drawLine(width()/2 - 80, (int)(height()*0.40),
               width()/2 + 80, (int)(height()*0.40));
}
