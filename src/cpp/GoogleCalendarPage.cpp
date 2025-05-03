/**
 * @class GoogleCalendarPage
 * @brief Implementation of the GoogleCalendarPage class, a custom WebViewPage for displaying Google Calendar content.
 * 
 * @details The GoogleCalendarPage class inherits from WebViewPage and is used to load and display Google Calendar content
 *          in a Qt WebView widget. The home URL is set to Google Calendar's homepage, and the class ensures touch events
 *          are accepted for interactive use on touch-enabled devices.
 * 
 * @author Amaan Hafeez, Mike Tran
 */

 #include "GoogleCalendarPage.h"

 /**
  * @brief Constructor for the GoogleCalendarPage class.
  * 
  * @details This constructor initializes the GoogleCalendarPage by setting its home URL and URL to Google Calendar's homepage.
  *          It also ensures that touch events are accepted by the widget for better interaction on touch-enabled devices.
  * 
  * @param parent Pointer to the parent QWidget, passed to the base class constructor (WebViewPage).
  */
 GoogleCalendarPage::GoogleCalendarPage(QWidget *parent) : WebViewPage(parent) {
     // Enable touch events for better user interaction on touch-enabled devices
     setAttribute(Qt::WA_AcceptTouchEvents, true);
 
     // Set the home URL to Google Calendar's homepage
     setHomeUrl(QUrl("https://calendar.google.com/"));
 
     // Set the initial URL to Google Calendar's homepage
     setUrl(QUrl("https://calendar.google.com/"));
 }
 
 /**
  * @brief Destructor for the GoogleCalendarPage class.
  * 
  * @details Currently, the destructor does not need to perform any specific cleanup operations, as the parent class
  *          handles resource management. The destructor is explicitly defined for clarity.
  * 
  * @returns void
  */
 GoogleCalendarPage::~GoogleCalendarPage() {}
 