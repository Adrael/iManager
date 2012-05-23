#------------------------------------------------#
#						 #
# Project created by Raphael MARQUES, 03/03/2012 #
#						 #
#------------------------------------------------#

QT         += core gui sql webkit
DEPENDPATH += . utils src

TARGET      = iPhoneManager5
TEMPLATE    = app

CONFIG     += console
RC_FILE     = utils/icon.rc

SOURCES    += src/main.cpp

HEADERS    += src/manager.hpp \
              src/sms.hpp \
    	      src/contacts.hpp \
    	      src/calls.hpp \
    	      src/notes.hpp \
    	      src/calendar.hpp \
    	      src/pictures.hpp \
    	      src/vault.hpp \
    	      src/webview.hpp \
    	      src/searchbar.hpp \
    	      src/people.hpp \
    	      src/database.hpp \
    	      src/menubar.hpp \
              src/configpages.hpp \
              src/configdialog.hpp \
    	      src/options.hpp \
    	      src/statusbar.hpp









