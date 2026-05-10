#include "ChessGUI.h"
#include <sstream>
ChessGUI::ChessGUI() : window(sf::VideoMode({ 1000u, 800u }), "Chess - SFML 3.0.2", sf::Style::Default)
{
    window.setFramerateLimit(60);
    sf::View view(sf::FloatRect({ 0.f, 0.f }, { 1000.f, 800.f }));
    window.setView(view);
    if (!font.openFromFile("C:/Windows/Fonts/seguisym.ttf"))
    {
        font.openFromFile("C:/Windows/Fonts/arial.ttf");
    }
    board.setupBoard();
}
void ChessGUI::run()
{
    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (event->is<sf::Event::Resized>())
            {
                sf::Vector2u size = window.getSize();
                sf::View view(sf::FloatRect({ 0.f, 0.f }, { (float)size.x, (float)size.y }));
                window.setView(view);
            }
            if (event->is<sf::Event::KeyPressed>())
            {
                auto key = event->getIf<sf::Event::KeyPressed>()->code;
                if (key == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }
            if (!gameOver && event->is<sf::Event::MouseButtonPressed>())
            {
                auto pos = sf::Mouse::getPosition(window);
                int row, col;
                if (pixelToBoard(pos.x, pos.y, row, col))
                {
                    handleClick(row, col);
                }
            }
        }
        window.clear(sf::Color(30, 30, 30));
        drawBoard();
        drawHighlights();
        drawPieces();
        drawPanel();
        if (gameOver)
        {
            drawGameOverOverlay();
        }
        window.display();
    }
}
void ChessGUI::buildMoveCache(int row, int col)
{
    cacheCount = 0;
    PIECE* p = board.get_piece(row, col);
    if (!p) return;
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (p->isValidMove(row, col, r, c, &board) && board.check_safety(row, col, r, c, currentTurn))
            {
                cacheRow[cacheCount] = r;
                cacheCol[cacheCount] = c;
                cacheCount++;
            }
        }
    }
}
void ChessGUI::handleClick(int row, int col)
{
    if (!pieceSelected)
    {
        PIECE* p = board.get_piece(row, col);

        if (p != nullptr && p->get_color() == currentTurn)
        {
            pieceSelected = true;
            selRow = row;
            selCol = col;
            statusMsg = "";
            buildMoveCache(row, col);
        }
    }
    else
    {
        if (row == selRow && col == selCol)
        {
            pieceSelected = false;
            selRow = selCol = -1;
            cacheCount = 0;
            return;
        }
        bool moveAllowed = false;
        for (int i = 0; i < cacheCount; i++)
        {
            if (cacheRow[i] == row && cacheCol[i] == col)
            {
                moveAllowed = true;
                break;
            }
        }
        if (moveAllowed)
        {
            board.movepiece(selRow, selCol, row, col);

            pieceSelected = false;
            selRow = selCol = -1;
            cacheCount = 0;

            postMoveLogic();
        }
        else
        {
            PIECE* dest = board.get_piece(row, col);

            if (dest != nullptr && dest->get_color() == currentTurn)
            {
                selRow = row;
                selCol = col;
                buildMoveCache(row, col);
            }
            else
            {
                pieceSelected = false;
                selRow = selCol = -1;
                cacheCount = 0;
            }
        }
    }
}
void ChessGUI::postMoveLogic()
{
    switchTurn();

    if (board.is_Check_Mate(currentTurn))
    {
        gameOver = true;
        COLOR winner = (currentTurn == WHITE) ? BLACK : WHITE;
        statusMsg = (winner == WHITE) ? "WHITE WINS!" : "BLACK WINS!";
    }
    else if (board.is_Check(currentTurn))
    {
        statusMsg = "CHECK!";
    }
    else
    {
        statusMsg = "";
    }
}
void ChessGUI::switchTurn()
{
    currentTurn = (currentTurn == WHITE) ? BLACK : WHITE;
}

float ChessGUI::getTileSize() const
{
    sf::Vector2u size = window.getSize();
    float boardW = (float)size.x;
    float boardH = (float)size.y;
    float tsX = boardW / 8.f;
    float tsY = boardH / 8.f;
    return (tsX < tsY) ? tsX : tsY;
}
void ChessGUI::drawBoard()
{
    float ts = getTileSize();
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            sf::Color color = ((r + c) % 2 == 0) ? lightSquare : darkSquare;
            sf::RectangleShape rect({ ts, ts });
            rect.setPosition({ c * ts, r * ts });
            rect.setFillColor(color);
            window.draw(rect);
        }
    }
}
void ChessGUI::drawPieces()
{
    float ts = getTileSize();
    unsigned int fontSize = (unsigned int)(ts * 0.65f);
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            PIECE* p = board.get_piece(r, c);
            if (!p)
            {
                continue;
            }
            std::wstring ws = getPieceSymbol(p->get_Symbol());
            sf::String sfStr(ws);
            sf::Text text(font, sfStr, fontSize);
            sf::Text shadow(font, sfStr, fontSize);
            if (p->get_color() == WHITE)
            {
                text.setFillColor(sf::Color::White);
            }
            else
            {
                text.setFillColor(sf::Color(20, 20, 20));
            }
            float x = c * ts;
            float y = r * ts;
            shadow.setFillColor(sf::Color(0, 0, 0, 100));
            shadow.setPosition({ x + 3, y + 3 });
            text.setPosition({ x, y });
            window.draw(shadow);
            window.draw(text);
        }
    }
}
void ChessGUI::drawPanel()
{
    float ts = getTileSize();
    float boardW = ts * 8;
    float winH = (float)window.getSize().y;
    float panelW = (float)window.getSize().x - boardW;
    float px = boardW;
    sf::RectangleShape panel({ panelW, winH });
    panel.setPosition({ px, 0.f });
    panel.setFillColor(sf::Color(40, 35, 30));
    window.draw(panel);
    sf::Text title(font, "CHESS", 32);
    title.setFillColor(sf::Color(220, 180, 100));
    title.setStyle(sf::Text::Bold);
    title.setPosition({ px + 40.f, 20.f });
    window.draw(title);
    sf::RectangleShape div({ panelW - 20.f, 2.f });
    div.setPosition({ px + 10.f, 70.f });
    div.setFillColor(sf::Color(100, 80, 50));
    window.draw(div);
    sf::Text turnLabel(font, "Turn:", 18);
    turnLabel.setFillColor(sf::Color(180, 180, 180));
    turnLabel.setPosition({ px + 15.f, 85.f });
    window.draw(turnLabel);
    sf::Text turnVal(font, (currentTurn == WHITE) ? "WHITE" : "BLACK", 22);
    turnVal.setFillColor((currentTurn == WHITE) ? sf::Color(255, 255, 255) : sf::Color(150, 120, 80));
    turnVal.setStyle(sf::Text::Bold);
    turnVal.setPosition({ px + 15.f, 110.f });
    window.draw(turnVal);
    if (!statusMsg.empty())
    {
        sf::RectangleShape statusBg({ panelW - 20.f, 60.f });
        statusBg.setPosition({ px + 10.f, 160.f });
        statusBg.setFillColor(sf::Color(180, 30, 30, 200));
        window.draw(statusBg);
        sf::Text statusText(font, statusMsg, 20);
        statusText.setFillColor(sf::Color::White);
        statusText.setStyle(sf::Text::Bold);
        statusText.setPosition({ px + 18.f, 170.f });
        window.draw(statusText);
    }
    sf::Text inst1(font, "Click piece to", 15);
    sf::Text inst2(font, "select, click", 15);
    sf::Text inst3(font, "square to move.", 15);
    inst1.setFillColor(sf::Color(140, 140, 140));
    inst2.setFillColor(sf::Color(140, 140, 140));
    inst3.setFillColor(sf::Color(140, 140, 140));
    inst1.setPosition({ px + 15.f, 270.f });
    inst2.setPosition({ px + 15.f, 292.f });
    inst3.setPosition({ px + 15.f, 314.f });
    window.draw(inst1);
    window.draw(inst2);
    window.draw(inst3);
    if (gameOver)
    {
        sf::Text hint(font, "Press ESC to quit", 15);
        hint.setFillColor(sf::Color(200, 160, 80));
        hint.setPosition({ px + 15.f, 360.f });
        window.draw(hint);
    }
}
void ChessGUI::drawTile(int row, int col, sf::Color color)
{
    float ts = getTileSize();
    sf::RectangleShape rect({ ts, ts });
    rect.setPosition({ col * ts, row * ts });
    rect.setFillColor(color);
    window.draw(rect);
}
std::wstring ChessGUI::getPieceSymbol(char c)
{
    switch (c)
    {
    case 'K': return L"\u2654";
    case 'Q': return L"\u2655";
    case 'R': return L"\u2656";
    case 'B': return L"\u2657";
    case 'N': return L"\u2658";
    case 'P': return L"\u2659";
    case 'k': return L"\u265A";
    case 'q': return L"\u265B";
    case 'r': return L"\u265C";
    case 'b': return L"\u265D";
    case 'n': return L"\u265E";
    case 'p': return L"\u265F";
    default:  return L"?";
    }
}
bool ChessGUI::pixelToBoard(int px, int py, int& row, int& col)
{
    float ts = getTileSize();
    if (px < 0 || py < 0 || px >= ts * 8 || py >= ts * 8)
    {
        return false;
    }
    col = px / ts;
    row = py / ts;
    return (row >= 0 && row < 8 && col >= 0 && col < 8);
}
void ChessGUI::drawGameOverOverlay()
{
    float ts = getTileSize();
    float boardW = ts * 8;
    float boardH = (float)window.getSize().y;
    sf::RectangleShape overlay({ boardW, boardH });
    overlay.setPosition({ 0.f, 0.f });
    overlay.setFillColor(sf::Color(0, 0, 0, 170));
    window.draw(overlay);
    float boxW = boardW * 0.7f;
    float boxH = 200.f;
    float boxX = (boardW - boxW) / 2.f;
    float boxY = (boardH - boxH) / 2.f;
    sf::RectangleShape border({ boxW + 6.f, boxH + 6.f });
    border.setPosition({ boxX - 3.f, boxY - 3.f });
    border.setFillColor(sf::Color(220, 180, 100));
    window.draw(border);
    sf::RectangleShape box({ boxW, boxH });
    box.setPosition({ boxX, boxY });
    box.setFillColor(sf::Color(30, 25, 20));
    window.draw(box);
    sf::Text title(font, "GAME OVER", 42);
    title.setFillColor(sf::Color(220, 180, 100));
    title.setStyle(sf::Text::Bold);
    auto tb = title.getLocalBounds();
    title.setOrigin({ tb.size.x / 2.f, 0.f });
    title.setPosition({ boardW / 2.f, boxY + 18.f });
    window.draw(title);
    sf::Text winner(font, statusMsg, 28);
    winner.setFillColor(sf::Color(255, 255, 255));
    winner.setStyle(sf::Text::Bold);
    auto wb = winner.getLocalBounds();
    winner.setOrigin({ wb.size.x / 2.f, 0.f });
    winner.setPosition({ boardW / 2.f, boxY + 90.f });
    window.draw(winner);
    sf::Text hint(font, "Press ESC to quit", 16);
    hint.setFillColor(sf::Color(160, 140, 100));
    auto hb = hint.getLocalBounds();
    hint.setOrigin({ hb.size.x / 2.f, 0.f });
    hint.setPosition({ boardW / 2.f, boxY + 155.f });
    window.draw(hint);
}
sf::Vector2f ChessGUI::getBoardOffset() const
{
    float ts = getTileSize();
    float boardSize = ts * 8.f;
    float availableWidth = window.getSize().x - PANEL;
    float offsetX = (availableWidth - boardSize) / 2.f;
    float offsetY = (window.getSize().y - boardSize) / 2.f;
    if (offsetX < 0)
    {
        offsetX = 0;
    }
    if (offsetY < 0)
    {
        offsetY = 0;
    }
    return { offsetX, offsetY };
}
void ChessGUI::drawHighlights()
{
    float ts = getTileSize();
    if (!pieceSelected)
    {
        return;
    }
    drawTile(selRow, selCol, selectColor);
    float dotR = ts * 0.15f;
    for (int i = 0; i < cacheCount; i++)
    {
        PIECE* target = board.get_piece(cacheRow[i], cacheCol[i]);
        bool isCapture = (target != nullptr && target->get_color() != currentTurn);
        if (isCapture)
        {
            drawTile(cacheRow[i], cacheCol[i], sf::Color(220, 50, 50, 160));
        }
        else
        {
            sf::CircleShape dot(dotR);
            dot.setFillColor(validColor);
            dot.setOrigin({ dotR, dotR });
            dot.setPosition({ cacheCol[i] * ts + ts / 2, cacheRow[i] * ts + ts / 2 });
            window.draw(dot);
        }
    }
}