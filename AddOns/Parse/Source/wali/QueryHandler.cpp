/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/WeightFactory.hpp"
#include "wali/QueryHandler.hpp"
#include "wali/wfa/WfaHandler.hpp"
#include "wali/wpds/WpdsHandler.hpp"
#include <xercesc/sax2/Attributes.hpp>
#include "StrX.hpp"
#include "wali/Key.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wfa/WFA.hpp"
#include <iostream>
#include <fstream>


namespace wali
{

    const std::string QueryHandler::XMLTag("Query");
    const std::string QueryHandler::XMLPoststarTag("poststar");
    const std::string QueryHandler::XMLPrestarTag("prestar");

    QueryHandler::QueryHandler( WeightFactory& wf ) :
        currentHandler(0),isPrestar(false)//,pdsHandler(wf),faHandler(wf)
    {
        pdsHandler = new wpds::WpdsHandler(wf);
        faHandler = new wfa::WfaHandler(wf);
        typeID = XMLString::transcode("type");
    }

    QueryHandler::QueryHandler( wpds::WpdsHandler* wpdsh , wfa::WfaHandler* wfah ) :
        currentHandler(0),isPrestar(false),pdsHandler(wpdsh),faHandler(wfah)
    {
        typeID = XMLString::transcode("type");
    }

    QueryHandler::~QueryHandler()
    {
        XMLString::release(&typeID);
        delete pdsHandler;
        delete faHandler;
    }

    bool QueryHandler::queryIsPrestar() const
    {
        return isPrestar;
    }
    wfa::WfaHandler& QueryHandler::getWfaHandler()
    {
        return *faHandler;
    }

    wpds::WpdsHandler& QueryHandler::getWpdsHandler()
    {
        return *pdsHandler;
    }

    wfa::WFA& QueryHandler::result()
    {
        return fa;
    }

    wfa::WFA& QueryHandler::run()
    {
        try {
            if( isPrestar ) {
                //pdsHandler.get().prestar(faHandler.get(),fa);
                pdsHandler->get().prestar(faHandler->get(),fa);
            }
            else {
                //pdsHandler.get().poststar(faHandler.get(),fa);
                pdsHandler->get().poststar(faHandler->get(),fa);
            }
        } catch(...) {
            std::cerr << "[ERROR] QueryHandler::run() caught an exception.\n";
            std::cerr << "\tCurrent status of resultant WFA := \n";
            fa.print( std::cerr ) << std::endl;
            std::ofstream of("QH.run.result.dot");
            fa.print_dot(of);
            of.close();
        }
        return fa;
    }

    //////////////////////////////////////////////////
    // Parsing handlers
    //////////////////////////////////////////////////

    void QueryHandler::startDocument()
    {
        // necessary?
        //pdsHandler.startDocument();
        //faHandler.startDocument();
        pdsHandler->startDocument();
        faHandler->startDocument();
    }

    void QueryHandler::endDocument()
    {
        // necessary?
        //pdsHandler.endDocument();
        //faHandler.endDocument();
        pdsHandler->endDocument();
        faHandler->endDocument();
    }

    void QueryHandler::startElement(  const   XMLCh* const    uri,
            const   XMLCh* const    localname,
            const   XMLCh* const    qname,
            const   Attributes&     attributes)
    {
        StrX who(localname);
        if( XMLTag == who.get() ) {
            StrX type = attributes.getValue(typeID);
            if( XMLPrestarTag == type.get() ) {
                isPrestar = true;
            }
            else if(XMLPoststarTag == type.get() ) {
                isPrestar = false;
            }
            else {
                assert(0);
            }
        }
        else if( wfa::WFA::XMLTag == who.get()) {
            //currentHandler = &faHandler;
            currentHandler = faHandler;
            currentHandler->startElement(uri,localname,qname,attributes);
        }
        else if( wpds::WPDS::XMLTag == who.get()) {
            //currentHandler = &pdsHandler;
            currentHandler = pdsHandler;
            currentHandler->startElement(uri,localname,qname,attributes);
        }
        else {
            assert(currentHandler);
            currentHandler->startElement(uri,localname,qname,attributes);
        }
    }

    void QueryHandler::endElement( const XMLCh* const uri,
            const XMLCh* const localname,
            const XMLCh* const qname)
    {
        StrX who(localname);
        if( XMLTag == who.get() ) {
            // nothing todo
            currentHandler = 0;
        }
        else if( (wfa::WFA::XMLTag == who.get()) || (wpds::WPDS::XMLTag == who.get())) {
            currentHandler->endElement(uri,localname,qname);
            currentHandler = 0;
        }
        else {
            assert(currentHandler);
            currentHandler->endElement(uri,localname,qname);
        }
    }

    //
    // All characters should be white space
    //
    void QueryHandler::characters(const XMLCh* const chars, const unsigned int length)
    {
        if( currentHandler ) {
            currentHandler->characters(chars,length);
        }
        else {
            // do nothing. Get calls to characters for all white space 
            // b/w elements
        }
    }

    void QueryHandler::ignorableWhitespace(                               
            const XMLCh* const chars
            , const unsigned int length
            )
    {
        if( currentHandler ) {
            currentHandler->ignorableWhitespace(chars,length);
        }
    }

    void QueryHandler::processingInstruction(   
            const XMLCh* const target
            , const XMLCh* const data
            )
    {
        if( currentHandler ) {
            currentHandler->processingInstruction(target,data);
        }
    }

    //////////////////////////////////////////////////
    // Default error handlers
    //////////////////////////////////////////////////

    void QueryHandler::warning(const SAXParseException& exc)
    {
        StrX msg(exc.getMessage());
        std::cerr << "[WARNING] " << msg << std::endl;
    }

    void QueryHandler::error(const SAXParseException& exc)
    {
        StrX msg(exc.getMessage());
        std::cerr << "[ERROR] " << msg << std::endl;
    }

    void QueryHandler::fatalError(const SAXParseException& exc)
    {
        StrX msg(exc.getMessage());
        std::cerr << "[FATAL ERROR] " << msg << std::endl;
        throw exc;
    }

    //////////////////////////////////////////////////
    // Helpers
    //////////////////////////////////////////////////

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
