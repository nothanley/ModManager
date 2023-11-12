/**
 * @file gamemanagerform.h
 * @author Hanleys
 * @brief Handles core logic for gamemanager interface.
 *  All game-specific child widgets such as tables, panels,
 *  labels, etc. are created and destroyed by this class.
 * @version 0.1
 * @date 2023-11-11
 * 
 * @copyright Copyright (c) 2023
 */


#ifndef GAMEMANAGERFORM_H
#define GAMEMANAGERFORM_H
#include <QWidget>
#include <QTableView>
#include <QGridLayout>

enum{
    GAME_WWE_23 = 0xbf5d9035,
    GAME_WWE_22 = 0xbf5d9034,
    TYPE_CHAR = 1110555230,
    TYPE_ARENA = 1110555198,
    TYPE_MISC = 1110555118,
    TYPE_MOVIE = 1110555142,
};

class GameCard;
class GameStatsTable;
class PreviewPanel;
class CGamePackage;
class CManagerController;
class CGameManager;
class AddItemDialog;

namespace Ui {
class GameManagerForm;
}

class GameManagerForm : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs Manager interface according to specified game GUID
     * 
     * @param gameHash  Globally unique identifier for manager's belonging game
     * @param parent Pointer to interface's parent widget. Inherits stylesheets and links deconstructor.
     */
    explicit GameManagerForm(const long long& gameHash, QWidget *parent = nullptr);
    ~GameManagerForm();

    long long m_GameHash;
    std::string m_GameTitle;

    /**
     * @brief Creates an item dialogue widget to add/edit mods, prompts user for details and updates registry with results
     * If user provides a valid GamePackage, the interface pre-populates with given data to edit selected mod.
     *
     * @param mod Pointer to CGamePackage, defaults as null.
     */
    void createItemDialog(CGamePackage* mod=nullptr);

private slots:
    void PopulatePreviewPanel(CGamePackage* selectedMod);
    void AddCardToLayout(CGamePackage* gameItem);
    void on_HomeLabel_clicked();
    void on_sizeDownButton_clicked();
    void on_sizeUpButton_clicked();
    void on_GameTopButton_2_clicked();
    void saveLocalConfigs();
    void focusItem(const QString name);

protected:
    /* todo: refactor non-relevant methods! */
    static const int CARD_WIDTH = 30 * 10;
    static const int CARD_HEIGHT = 18 * 10;
    static constexpr float GRID_SCALE = 13;
    static constexpr float MIN_GRID_SCALE = 44;
    static constexpr float MAX_GRID_SCALE = 90;
    static const int DEBUG_TOTAL_CARDS = 11;
    double m_CustomGridScale = 44;

    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void ResetLayout(); /* Clears existing UI elements */
    void RefreshGameStats();
    void RefreshRibbonStats();
    void RefreshAll(); /* Repopulates tables and grids */
    void ClearGrid();

    /**
     * @brief Open's selected package in explorer page. Closes manager window and focuses Explorer GUI.
     * @param Selected package to view contents from.
     */
    void openPakInExplorer(CGamePackage* mod);

    /**
     * @brief Repopulates item's in profile combo widget with active manager's profile list
     */
    void refreshProfileComboBox();

    /**
     * @brief Populates grid interface with stylized card widgets based on active manager profile
     * 
     */
    void PopulateCardGrid();
    
    /**
     * @brief Checks if manager interface has an already existing dialogue widget for adding/editing mods.
     * 
     * @return true 
     * @return false 
     */
    bool hasItemDialog();

    /**
     * @brief Closes the existing item dialog widget and updates member pointer. Dialog deletion self-handled by widget upon close.
     * 
     */
    void clearItemDialog();

    /**
     * @brief Gets non-specific layout for specified manager game.
     * 
     */
    void GetManagerLayoutGeneral();

    /**
     * @brief Initializes interface with configuration JSON located in application's roaming path. Loads all managers/profiles.
     * 
     */
    void InitializeManagerSettings();

    /**
     * @brief Sets up UI and manager. Focuses requested game GUID (provided in constructor).
     * 
     */
    void PopulateManagerGUI();

    /**
     * @brief Creates child widget table depicting currently selected item's properties.
     * 
     */
    void InitializeStatsTable();

    /**
     * @brief Creates child widget panel depicting item properties, previews, and a visual listing for the active profile's load order.
     */
    void InitializePreviewPanel();

    /**
     * @brief Checks if active manager has a valid JSON configuration. Creates generic manager if no valid manager's are found.
     * 
     * @param gameName Game title used in specifying manager to validate
     * @return true 
     * @return false 
     */
    bool ValidateManager(const char* gameName, const bool override=true);

    /**
     * @brief Create a generic Yes/No Dialog widget for user prompt.
     * */
    void createYesNoDialog(const char* prompt, bool* ok);

    Ui::GameManagerForm *ui;
    CManagerController* m_CTRLManager = nullptr;
    CGameManager* pGameManager = nullptr;
    PreviewPanel* pPreviewPanel = nullptr;
    AddItemDialog* pItemDialog = nullptr;
    GameStatsTable* pStatsTable = nullptr;
    std::vector<GameCard*> pGameCards;


private:
    void addZippedMod(const QString &path);
    void addCakeMod(const QString &path);
    CGamePackage* getZippedGamePak(const QString &zipPath, const QString &appPath);
    bool moveModtoManagerRoot(CGamePackage* gameMod, const QString& path );
    void addToActiveProfile(CGamePackage* mod);
    CGamePackage* createGameZipPrompt(CGamePackage* gameMod);
    void removeProfileMod(CGamePackage* mod);

    friend GameCard;
};

#endif // GAMEMANAGERFORM_H











