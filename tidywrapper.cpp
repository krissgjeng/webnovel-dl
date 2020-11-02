#include "tidywrapper.h"
#include <tidy/tidy.h> //fix this on windows
#include <tidy/tidybuffio.h>
#include <stdio.h>
#include <errno.h>
#include <sstream>
#include <iostream>

TidyWrapper::TidyWrapper(){}

std::string TidyWrapper::tidyhtmlToXHtml(std::string input)
{
  std::cout << "tidying input\n";
  std::string qsoutput;
  //const char*  cinput = "<title>Foo</title><p>Foo!";// input.toUtf8();
  bool ok;
  int rc = 0;
  TidyBuffer output = {nullptr};
  //TidyBuffer errbuf = {nullptr};

  TidyDoc tdoc = tidyCreate();

  ok = tidyOptSetBool(tdoc, TidyXhtmlOut, yes); // Convert to XHTML
                                                //   tidyOptSetValue(tdoc, TidyInCharEncoding, "UTF16");
                                                //   tidyOptSetValue(tdoc, TidyOutCharEncoding, "UTF8");
  tidyOptSetBool(tdoc, TidyPreserveEntities, yes);
  //tidyOptSetBool(tdoc, TidyQuoteMarks, yes);
  tidyOptSetBool(tdoc, TidyIndentAttributes, yes);
  tidyOptSetBool(tdoc, TidyXmlDecl, yes);
  tidyOptSetBool(tdoc, TidyXmlSpace, yes);
  //tidyOptSetValue(tdoc, TidyDoctype, "-//W3C//DTD XHTML 1.1//EN"); // \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd");
  tidyOptSetValue(tdoc, TidyDoctype, "loose");
  tidyOptSetBool(tdoc, TidyOptionId::TidyStyleTags, yes);
  tidyOptSetBool(tdoc, TidyOptionId::TidyEncloseBodyText, yes);
  tidyOptSetBool(tdoc, TidyOptionId::TidyEscapeCdata, yes);
  tidyOptSetBool(tdoc, TidyOptionId::TidyEscapeScripts, yes);
  tidyOptSetValue(tdoc, TidyOptionId::TidyWrapLen, "0");
  tidyOptSetBool(tdoc, TidyOptionId::TidyJoinClasses, yes);
  tidyOptSetBool(tdoc, TidyOptionId::TidyJoinStyles, yes);
  tidyOptSetBool(tdoc, TidyOptionId::TidyDropPropAttrs, yes);
  tidyOptSetBool(tdoc, TidyOptionId::TidyShowInfo, no);
  tidyOptSetBool(tdoc, TidyOptionId::TidyShowWarnings, no);
  tidyOptSetBool(tdoc, TidyOptionId::TidyShowMetaChange, no);
  tidyOptSetBool(tdoc, TidyOptionId::TidyQuiet, yes);
  //tidyOptSetBool(tdoc, TidyOptionId::TidyBodyOnly, yes);
  //if ( ok )
  //rc = tidySetErrorBuffer( tdoc, &errbuf );      // Capture diagnostics
  if (rc >= 0)
    rc = tidyParseString(tdoc, input.c_str()); //input.toUtf8() );           // Parse the input
  if (rc >= 0)
    rc = tidyCleanAndRepair(tdoc); // Tidy it up!
  //if ( rc >= 0 )
  //rc = tidyRunDiagnostics( tdoc );               // Kvetch
  //if ( rc > 1 )                                    // If error, force output.
  //rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );
  if (rc >= 0)
    rc = tidySaveBuffer(tdoc, &output); // Pretty Print

  if (rc >= 0)
  {
    //if ( rc > 0 )
    //printf( "\nDiagnostics:\n\n%s", errbuf.bp );
    //printf( "\nAnd here is the result:\n\n%s", output.bp );
    std::stringbuf str;
    std::ostream os(&str);
    os << output.bp;
    qsoutput = std::string(str.str());
    //qDebug() << "The Result: "<< qsoutput;
  }
  else
    printf("A severe error (%d) occurred.\n", rc);

  tidyBufFree(&output);
  //tidyBufFree( &errbuf );
  tidyRelease(tdoc);
  return qsoutput;
  //return input;
}
