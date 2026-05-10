#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
class ChessGUI
{
private:
    sf::RenderWindow window;
    sf::Font font;
    static const int TILE = 80;
    static const int PANEL = 200;
    sf::Color lightSquare{ 240, 217, 181 };
    sf::Color darkSquare{ 181, 136, 99 };
    sf::Color selectColor{ 50, 200, 50, 160 };
    sf::Color validColor{ 100, 200, 255, 130 };
    sf::Color checkColor{ 220, 50, 50, 180 };
    Board board;
    COLOR currentTurn = WHITE;
    bool pieceSelected = false;
    int selRow = -1, selCol = -1;
    bool gameOver = false;
    std::string statusMsg = "";
    int cacheRow[64];
    int cacheCol[64];
    int cacheCount = 0;
    void handleClick(int row, int col);
    void drawBoard();
    void drawPieces();
    void drawHighlights();
    void drawPanel();
    void drawGameOverOverlay();
    void drawTile(int row, int col, sf::Color color);
    float getTileSize() const;
    std::wstring getPieceSymbol(char c);
    bool pixelToBoard(int px, int py, int& row, int& col);
    void postMoveLogic();
    void switchTurn();
    void buildMoveCache(int row, int col);
    sf::Vector2f getBoardOffset() const;
public:
    ChessGUI();
    void run();
};