cmake_minimum_required(VERSION 3.5)
include(CMakePrintHelpers)

project(qDocBase6 VERSION 6.0.0 LANGUAGES CXX DESCRIPTION "Document DataBase Manager")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_FOLDER src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${SRC_FOLDER})
cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)


# find_package(Qt6 REQUIRED COMPONENTS Pdf)
# target_link_libraries(mytarget Qt6::Pdf)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Gui Core Widgets LinguistTools
    Sql Network Xml Concurrent PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Core Widgets LinguistTools
    Sql Network Xml Concurrent PrintSupport)

set(TS_FILES ${SRC_FOLDER}/qDocBase6_en_US.ts ${SRC_FOLDER}/qDocBase6_pt_BR.ts)

set(PROJECT_SOURCES
        ${SRC_FOLDER}/main.cc
        ${SRC_FOLDER}/qdocbase.cc
        ${SRC_FOLDER}/qdocbase.h
        ${SRC_FOLDER}/qdocbase.ui
        ${SRC_FOLDER}/formentities.cc
        ${SRC_FOLDER}/formentities.h
        ${SRC_FOLDER}/formentities.ui
        ${SRC_FOLDER}/formdocuments.cc
        ${SRC_FOLDER}/formdocuments.h
        ${SRC_FOLDER}/formdocuments.ui
        ${SRC_FOLDER}/formsearch.cc
        ${SRC_FOLDER}/formsearch.h
        ${SRC_FOLDER}/formsearch.ui
        ${SRC_FOLDER}/dialogsettings.cc
        ${SRC_FOLDER}/dialogsettings.h
        ${SRC_FOLDER}/dialogsettings.ui
        ${SRC_FOLDER}/formtopsecret2.cc
        ${SRC_FOLDER}/formtopsecret2.h
        ${SRC_FOLDER}/formtopsecret2.ui
        ${SRC_FOLDER}/dialogabout.cc
        ${SRC_FOLDER}/dialogabout.h
        ${SRC_FOLDER}/dialogabout.ui
        ${SRC_FOLDER}/modules_listview_p.cc
        ${SRC_FOLDER}/modules_listview_p.h
        ${SRC_FOLDER}/customuppervalidator.cc
        ${SRC_FOLDER}/customuppervalidator.h
        ${SRC_FOLDER}/customlowervalidator.cc
        ${SRC_FOLDER}/customlowervalidator.h
        ${SRC_FOLDER}/customdoublevalidator.cc
        ${SRC_FOLDER}/customdoublevalidator.h
        ${SRC_FOLDER}/lineedit_fieldchecking.cc
        ${SRC_FOLDER}/lineedit_fieldchecking.h
        ${SRC_FOLDER}/combobox_phones_p.cc
        ${SRC_FOLDER}/combobox_phones_p.h
        ${SRC_FOLDER}/combobox_entities_p.cc
        ${SRC_FOLDER}/combobox_entities_p.h
        ${SRC_FOLDER}/departments_p.cc
        ${SRC_FOLDER}/departments_p.h
        ${SRC_FOLDER}/doctypes_p.cc
        ${SRC_FOLDER}/doctypes_p.h
        ${SRC_FOLDER}/filefilters_p.cc
        ${SRC_FOLDER}/filefilters_p.h
        ${SRC_FOLDER}/baseentities_p.cc
        ${SRC_FOLDER}/baseentities_p.h
        ${SRC_FOLDER}/storageentities_p.cc
        ${SRC_FOLDER}/storageentities_p.h
        ${SRC_FOLDER}/tableview_documents_p.cc
        ${SRC_FOLDER}/tableview_documents_p.h
        ${SRC_FOLDER}/tableview_documents_itemdelegate.cc
        ${SRC_FOLDER}/tableview_documents_itemdelegate.h
        ${SRC_FOLDER}/tableview_results_itemdelegate.cc
        ${SRC_FOLDER}/tableview_results_itemdelegate.h
        ${SRC_FOLDER}/typeimage.cc
        ${SRC_FOLDER}/typeimage.h
        ${SRC_FOLDER}/multifiles.cc
        ${SRC_FOLDER}/multifiles.h
        ${SRC_FOLDER}/globals.cc
        ${SRC_FOLDER}/globals.h
        ${SRC_FOLDER}/br_cnpj.cc
        ${SRC_FOLDER}/br_cnpj.h
        ${SRC_FOLDER}/br_cpf.cc
        ${SRC_FOLDER}/br_cpf.h
        ${SRC_FOLDER}/progid.cc
        ${SRC_FOLDER}/progid.h
        ${SRC_FOLDER}/settings.cc
        ${SRC_FOLDER}/settings.h
        ${SRC_FOLDER}/sysactivity.cc
        ${SRC_FOLDER}/sysactivity.h
        ${SRC_FOLDER}/abstractprint.cc
        ${SRC_FOLDER}/abstractprint.h
        ${SRC_FOLDER}/printimage.cc
        ${SRC_FOLDER}/printimage.h
        ${SRC_FOLDER}/printmultiplesimages.cc
        ${SRC_FOLDER}/printmultiplesimages.h
        ${SRC_FOLDER}/printdocanalitic.cc
        ${SRC_FOLDER}/printdocanalitic.h
        ${SRC_FOLDER}/printdocgeneral.cc
        ${SRC_FOLDER}/printdocgeneral.h
        ${SRC_FOLDER}/temporaryfiles.cc
        ${SRC_FOLDER}/temporaryfiles.h
        ${SRC_FOLDER}/exportfiles.cc
        ${SRC_FOLDER}/exportfiles.h
        ${SRC_FOLDER}/importbatch.cc
        ${SRC_FOLDER}/importbatch.h
        ${SRC_FOLDER}/messages.cc
        ${SRC_FOLDER}/messages.h
        ${SRC_FOLDER}/qdocbase.qrc
        ${SRC_FOLDER}/abstracttr.cc
        ${SRC_FOLDER}/abstracttr.h
        ${SRC_FOLDER}/formimportbatch.h
        ${SRC_FOLDER}/formimportbatch.cc
        ${SRC_FOLDER}/formimportbatch.ui
        ${SRC_FOLDER}/formlogimportbatch.h
        ${SRC_FOLDER}/formlogimportbatch.cc
        ${SRC_FOLDER}/formlogimportbatch.ui
        ${SRC_FOLDER}/tablewidgetsheet_import_itemdelegate.cc
        ${SRC_FOLDER}/tablewidgetsheet_import_itemdelegate.h
        ${SRC_FOLDER}/tablewidget_sheet_p.cc
        ${SRC_FOLDER}/tablewidget_sheet_p.h
        README.md
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qDocBase6
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}        
    )
    qt_create_translation(QM_FILES ${SRC_FOLDER}/${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(qDocBase6 PRIVATE
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Concurrent
    Qt${QT_VERSION_MAJOR}::PrintSupport
    #Qt${QT_VERSION_MAJOR}::Pdf
)

set_target_properties(qDocBase6 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS qDocBase6
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# cmake_print_variables(CMAKE_INSTALL_LIBDIR)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qDocBase6)
endif()
