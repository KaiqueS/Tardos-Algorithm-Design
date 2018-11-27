#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ){

	logic = new Program_Logic<char>();

	ui -> setupUi( this );

	scene = new QGraphicsScene( this );

	ui -> screen -> setScene( scene );

	ui->actionLoad->setToolTip(tr("Load contacts from a file"));

	ui->actionSave->setToolTip(tr("Save contacts to a file"));

	fps = new QTimer( this );


	controle = new QTimer( this );

	// Connect timer
	connect( fps, SIGNAL( timeout( ) ), this, SLOT( update( ) ) );

	fps -> start( 20 );

}

MainWindow::~MainWindow( ){

	delete ui;
}

void MainWindow::on_set_dimension_button_clicked( ){

	delete logic;

	logic = new Program_Logic<char>();

	int row = ui -> row_size_input -> value( );

	int column = ui -> column_size_input -> value( );

	std::vector<SocialPrefNode> graph = logic->run_project( row, column );

	scene -> clear( );

	logic -> show_graph( graph, scene );

	logic -> rank();

}

void MainWindow::update( ){

    logic -> update( ui -> isMagnetic -> isChecked( ) );
}

void MainWindow::on_actionSave_triggered( ){

	QString filename = QFileDialog::getSaveFileName( this, tr( "Save Address Book" ), "", tr( "Address Book( *.sctb );; All Files( * )" ) );

	QFile file( filename );

	if( !file.open( QIODevice::WriteOnly ) ){

		std::cout << "Could not open " << filename.toStdString( ) << std::flush;

		// Hum...
		return;
	}

	QDataStream out( &file );
	out.setVersion( QDataStream::Qt_5_1 );

	std::vector<Agent> listofagents = logic -> get_list_of_agents( );

    int sizeList = static_cast<int>( listofagents.size( ) );

	out << sizeList;

	for( Agent agent: listofagents ){

		out << QString::fromStdString( agent.get_id( ) );

		std::vector<Options> preferences = agent.get_preferences( );

        int sizePref = static_cast<int>( preferences.size( ) );

		out << sizePref;

		for( Options pref : preferences ){

			out << QString::fromStdString( pref.get_opt( ) );
			out << pref.get_value( );
		}
	}

	file.flush( );
	file.close( );
}

void MainWindow::on_actionLoad_triggered( ){

	delete logic;

	logic = new Program_Logic<char>( );

	QString filename = QFileDialog::getOpenFileName( this, tr( "Open Address Book" ), " ", tr( "Address Book( *.sctb );; All Files( * )" ) );

	QFile file( filename );

	if( !file.open( QIODevice::ReadOnly ) ){

		std::cout << "Could not open " << filename.toStdString( ) << std::flush;

		// Hum...
		return;
	}

	QDataStream in( &file );
	in.setVersion( QDataStream::Qt_5_1 );

	std::vector<Agent> listofagents{ };

	int sizeList{ };

	in >> sizeList;

	int prefSize = 0;

	for( int i = 0; i < sizeList; i++ ){

		QString id{ };

		in >> id;

		std::vector<Options> preferences{ };

		int sizePref{ };

		in >> sizePref;

		for( int i = 0; i < sizePref; i++ ){

			int alt{ };
			int val{ };

			in >> alt;
			in >> val;

			Options opt( std::to_string( alt ), val );

			preferences.push_back( opt );
		}

        if( prefSize<static_cast<int>( preferences.size( ) ) )

			prefSize = static_cast<int>( preferences.size( ) );

		listofagents.push_back( Agent( preferences, id.toStdString( ) ) );
	}

	std::vector<SocialPrefNode> graph = logic -> run_project( listofagents );

	logic -> setListofagents( listofagents );

	ui -> row_size_input -> setValue( static_cast<int>( listofagents.size( ) ) ) ;

	ui -> column_size_input -> setValue( prefSize );

	file.close( );

	scene -> clear( );

	logic -> show_graph( graph, scene );

	logic -> rank( );
}

void MainWindow::on_actionimport_triggered( ){

	delete logic;

	logic = new Program_Logic<char>( );

	QString filename = QFileDialog::getOpenFileName( this, tr( "Open Address Book" ), " ", tr( "Address Book( *.csv );; All Files( * )" ) );

	QFile file( filename );

	if( !file.open( QIODevice::ReadOnly ) ){

		std::cout << "Could not open " << filename.toStdString( ) << std::flush;

		// Hum...
		return;
	}

	QDataStream in( &file );
	in.setVersion( QDataStream::Qt_5_1 );

	std::vector<Agent> listofagents{ };

	QString entrada{ };

	int bufferSize = 100;

	// TODO: See how to deal with this
	char buffer[ bufferSize ];

	while( !in.atEnd( ) ){

		int size{ };

		size = in.readRawData( buffer, bufferSize );

		if( size < bufferSize )

			for( int i = size; i < bufferSize; i++ )

				buffer[ i ] = 0;

		entrada += QString( buffer );
	}

	QStringList listStringNode = entrada.split( QString( "\n" ) );
	QStringList listStringHeader = listStringNode.at( 0 ).split( QString( ";" ) );

	std::vector<Options> preferences{ };

	int prefSize{ 0 };

	for( int i = 1; i < listStringNode.size( ) - 1; i++ ){

		QString stringNode = listStringNode.at( i );

		QStringList listStringOpts = stringNode.split( QString( ";" ) );

		QString id = listStringOpts.at( 0 );

		std::vector<Options> preferences{ };

		for( int j = 1; j < listStringOpts.size( ) ; j++ ){

			char id = '_';

			if( j < listStringHeader.size( ) )

				id = listStringHeader.at( j ).toStdString( )[ 0 ];

			int val = listStringOpts.at( j ).toInt( );

			Options opt( std::to_string( id ), val );

			preferences.push_back( opt );
		}

		if( prefSize < static_cast<int>( preferences.size( ) ) )

			prefSize = static_cast<int>( preferences.size( ) );

		listofagents.push_back( Agent( preferences, id.toStdString( ) ) );
	}

	std::vector<SocialPrefNode> graph = logic -> run_project( listofagents );

	logic -> setListofagents( listofagents );

	ui -> row_size_input -> setValue( static_cast<int>( listofagents.size( ) ) );
	ui -> column_size_input -> setValue( prefSize );

	file.close( );

	scene -> clear( );

	logic -> show_graph( graph, scene );

	logic -> rank( );

}

void MainWindow::on_actionExport_triggered( ){

	QString filename = QFileDialog::getSaveFileName( this, tr( "Save Address Book" ), "", tr( "Address Book( *.csv );; All Files( * )" ) );

	QFile file( filename );

	if( !file.open( QIODevice::WriteOnly ) ){

		std::cout << "Could not open " << filename.toStdString( ) << std::flush;

		// Hum...
		return;
	}

	file.close( );

	std::ofstream myfile{ };

	myfile.open( filename.toStdString( ) );

	std::vector<Agent> listofagents{ };

	std::string saida( "" );

	std::vector<Agent> listAgent = logic -> get_list_of_agents( );

	saida += "idAgents\\idOptions";

	std::vector<Options> preferencesaux{ };

	for( Options opt : listAgent.at( 0 ).get_preferences( ) ){

		saida += ";";
		saida += opt.get_opt( );
	}

	for( Agent agent : listAgent ){

		saida += "\n";

		std::string str = agent.get_id( );

		saida += str;

		for( Options opt : agent.get_preferences( ) ){

			saida += ";";
			saida += QString::number( opt.get_value( ) ).toStdString( );
		}
	}

	myfile << saida << "\n";

	myfile.close( );
}

void MainWindow::on_actionAgents_triggered( ){

	if( window != nullptr )

		delete window;

	window = new QWidget( );

	window -> resize( 320, 240 );
	window -> setWindowTitle( "Agents" );
	window -> show( );

	// Check for DELETE
	QVBoxLayout* layout = new QVBoxLayout;

	window -> setLayout( layout );

	// Check for DELETE
	tree = new TreeWidget( window, 2 );

	layout -> addWidget( tree -> getTree( ) );

	for( Agent a: logic -> get_list_of_agents( ) ){

		QTreeWidgetItem* agentItem = tree -> addTreeRoot( QString::fromStdString( a.get_id( ) ) , "Agent" );

		for( Options opt : a.get_sorted_preferences( ) ){

			std::string alternId{ opt.get_opt( ) };

			QTreeWidgetItem* optionItem = tree -> addTreeChild( agentItem , QString::fromStdString( alternId ) , "Alternative" );

			tree -> addTreeChild( optionItem , QString::number( opt.get_value( ) ) , "Value" );
		}
	}
}

void MainWindow::on_actionSocial_Preferencies_triggered( ){

	if( window!= nullptr )

		delete window;

	window = new QWidget( );

	window -> resize( 320, 240 );

	window -> setWindowTitle( "Social Preferences" );

	window -> show( );

	// Check for { } default initialization
	QVBoxLayout* layout = new QVBoxLayout{ };

	window -> setLayout( layout );

	tree = new TreeWidget( window, 2 );

	layout -> addWidget( tree -> getTree( ) );

	if( logic -> getSocialPrefGraph( ) )

		for( SocialPrefNode opt: *( logic -> getSocialPrefGraph( ) ) ){

			std::string alternId{ opt.get_id( ) };

			QTreeWidgetItem* optItem = tree -> addTreeRoot( QString::fromStdString( alternId ) , "Alternative" );
			QTreeWidgetItem* prefItem = tree -> addTreeChild( optItem , "Preferred" , "Relation" );

			for( SocialPrefNode* preferred : logic -> spnGetFromList( opt.get_preferences( ) ) ){

				std::string alternIdAux{ preferred->get_id() };

				tree -> addTreeChild( prefItem , QString::fromStdString( alternIdAux ) , "Alternative" );
			}

			QTreeWidgetItem* worseItem = tree -> addTreeChild( optItem , "Worse" , "Relation" );

			for( SocialPrefNode* worse : logic -> spnGetFromList( opt.get_worse( ) ) ){

				std::string alternIdAux{ worse->get_id( ) };

				tree -> addTreeChild( worseItem , QString::fromStdString( alternIdAux ) , "Alternative" );
			}

			QTreeWidgetItem* equalItem = tree -> addTreeChild( optItem , "Equal" , "Relation" );

			for( SocialPrefNode* equal : logic -> spnGetFromList( opt.get_indiff( ) ) ){

				std::string alternIdAux{ equal -> get_id( ) };

				tree -> addTreeChild( equalItem , QString::fromStdString( alternIdAux ) , "Alternative" );
			}
		}
}
