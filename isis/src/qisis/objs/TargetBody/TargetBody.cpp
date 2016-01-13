#include "TargetBody.h"

#include <QDataStream>
#include <QDebug>
#include <QList>
#include <QUuid>
#include <QXmlStreamWriter>

#include <hdf5.h>
#include <hdf5_hl.h> // in the hdf5 library
#include <hdf5.h>

#include "Distance.h"
#include "IString.h"
#include "Project.h"
#include "PvlKeyword.h"
#include "PvlObject.h"
//#include "Target.h"
#include "TargetBodyDisplayProperties.h"
#include "XmlStackedHandlerReader.h"

namespace Isis {

  TargetBody::TargetBody(Target *target, QObject *parent) : QObject(parent) {
    m_id = NULL;

    m_radii.resize(3, Distance());
    m_sigmaRadii.resize(3, Distance(0.0, Distance::Kilometers));

    m_bodyCode = new SpiceInt;
    m_systemCode = new SpiceInt;

    m_systemName = target->systemName();

    // TODO - initialize TargetBody members from target
    *m_bodyCode = target->naifBodyCode();
    *m_systemCode = target->naifPlanetSystemCode();
    m_radii[0] = target->radii().at(0);
    m_radii[1] = target->radii().at(1);
    m_radii[2] = target->radii().at(2);

    m_raPole = target->poleRaCoefs();
    m_decPole = target->poleDecCoefs();
    m_pm = target->pmCoefs();

    m_raNutPrec = target->poleRaNutPrecCoefs();
    m_decNutPrec = target->poleDecNutPrecCoefs();
    m_pmNutPrec = target->pmNutPrecCoefs();

    m_sysNutPrec0 = target->sysNutPrecConstants();
    m_sysNutPrec1 = target->sysNutPrecCoefs();

    m_displayProperties
        = new TargetBodyDisplayProperties(target->name(), this);

    m_id = new QUuid(QUuid::createUuid());
  }



  TargetBody::TargetBody(BundleTargetBodyQsp bundleTargetBody, QObject *parent) : QObject(parent) {
    m_id = NULL;

    m_radii.resize(3, Distance());
    m_sigmaRadii.resize(3, Distance(0.0, Distance::Kilometers));

    m_bodyCode = new SpiceInt;
    m_systemCode = new SpiceInt;

//    m_systemName = bundleTargetBody->systemName();

    // TODO - initialize TargetBody members from bundleTargetBody
//    *m_bodyCode = target->naifBodyCode();
//    *m_systemCode = target->naifPlanetSystemCode();
    m_radii = bundleTargetBody->radii();
//    m_sigmaRadii = bundleTargetBody->

    m_raPole = bundleTargetBody->poleRaCoefs();
    m_decPole = bundleTargetBody->poleDecCoefs();
    m_pm = bundleTargetBody->pmCoefs();

//    m_raNutPrec = bundleTargetBody->poleRaNutPrecCoefs();
//    m_decNutPrec = bundleTargetBody->poleDecNutPrecCoefs();
//    m_pmNutPrec = bundleTargetBody->pmNutPrecCoefs();

//    m_sysNutPrec0 = bundleTargetBody->sysNutPrecConstants();
//    m_sysNutPrec1 = bundleTargetBody->sysNutPrecCoefs();

//    m_displayProperties
//        = new TargetBodyDisplayProperties(target->name(), this);

    m_id = new QUuid(QUuid::createUuid());
  }


  //  TargetBody::TargetBody(Project *project, XmlStackedHandlerReader *xmlReader,
//                         QObject *parent) : QObject(parent) {   // TODO: does xml stuff need project???
//    m_id = NULL;

//    xmlReader->pushContentHandler(new XmlHandler(this, project));
//    xmlReader->setErrorHandler(new XmlHandler(this, project));
//  }



//  TargetBody::TargetBody(const TargetBody &src)
//      : m_id(new QUuid(src.m_id->toString())) {
//
//    m_bodyCode = new SpiceInt(*src.m_bodyCode);
//
//    m_radii.resize(3, Distance());
//    m_sigmaRadii.resize(3, Distance(3.0, Distance::Kilometers)); // TODO - radii sigma fudged for now
//
//    for (int i = 0; i < 3; i++) {
//      m_radii[i] = src.m_radii[i];
//      m_sigmaRadii[i] = src.m_sigmaRadii[i];
//    }
//
//    m_displayProperties
//        = new TargetBodyDisplayProperties(*src.m_displayProperties);
//
//    int fred=1;
//    m_bodyCode = src.m_bodyCode;
//  }



  TargetBody::~TargetBody() {
    delete m_id;
    m_id = NULL;

    delete m_bodyCode;
    m_bodyCode = NULL;

    delete m_systemCode;
    m_systemCode = NULL;
  }


  
//  TargetBody &TargetBody::operator=(const TargetBody &src) {

//    if (&src != this) {

//      delete m_id;
//      m_id = NULL;
//      m_id = new QUuid(src.m_id->toString());
//    }

//    return *this;
//  }


  /**
   * Compares two Target Body objects to see if they are equal
   *
   * @param src TargetBody object to compare against
   *
   * @return bool Returns true if the objects are equal, false if not
   */
  bool TargetBody::operator== (const TargetBody &src) const {

    TargetBody *rtargetBody = (TargetBody *) &src;

    if (*m_bodyCode != rtargetBody->naifBodyCode())
      return false;
    if (m_radii[0] != rtargetBody->radiusA())
      return false;
    if (m_radii[1] != rtargetBody->radiusB())
      return false;
    if (m_radii[2] != rtargetBody->radiusC())
      return false;

    return true;
  }


  TargetBodyDisplayProperties *TargetBody::displayProperties() {
    return m_displayProperties;
  }


  const TargetBodyDisplayProperties *TargetBody::displayProperties() const {
    return m_displayProperties;
  }


//  PvlObject TargetBody::pvlObject(QString resultsName, QString settingsName,
//                                     QString statisticsName) {

//    PvlObject pvl(resultsName);
//    pvl += PvlKeyword("RunTime", runTime());
//    if (m_controlNetworkFileName->expanded() != "") {
//      pvl += PvlKeyword("OutputControlNetwork", controlNetworkFileName());
//    }
//    pvl += bundleSettings()->pvlObject(settingsName);
//    pvl += bundleResults()->pvlObject(statisticsName);
//    return pvl;

//  }

  std::vector<Angle> TargetBody::poleRaCoefs() {
    return m_raPole;
  }


  std::vector<Angle> TargetBody::poleDecCoefs() {
    return m_decPole;
  }


  std::vector<Angle> TargetBody::pmCoefs() {
    return m_pm;
  }


  std::vector<double> TargetBody::poleRaNutPrecCoefs() {
    return m_raNutPrec;
  }


  std::vector<double> TargetBody::poleDecNutPrecCoefs() {
    return m_decNutPrec;
  }


  std::vector<double> TargetBody::pmNutPrecCoefs() {
    return m_pmNutPrec;
  }


  std::vector<Angle> TargetBody::sysNutPrecConstants() {
    return m_sysNutPrec0;
  }


  std::vector<Angle> TargetBody::sysNutPrecCoefs() {
    return m_sysNutPrec1;
  }


  /**
   * This returns the NAIF body code of the target
   *
   * @return @b SpiceInt NAIF body code
   *
   */
  SpiceInt TargetBody::naifBodyCode() const {
    return *m_bodyCode;
  }


  /**
   * This returns the NAIF body code of the target's planet system
   *
   * @return @b SpiceInt NAIF body code of target's planet system
   *
   */
  SpiceInt TargetBody::naifPlanetSystemCode() const {
    return *m_systemCode;
  }


  /**
   * This returns the body name of the target's planet system
   *
   * @return @b QString body name of target's planet system
   *
   */
  QString TargetBody::naifPlanetSystemName() const {
    return m_systemName;
  }


  /**
   * returns "a" radius
   *
   * @return Distance value of body radius "a"
   */
  Distance TargetBody::radiusA() const {
    return m_radii[0];
  }


  /**
   * returns "a" radius sigma
   *
   * @return Distance value of body radius "a" sigma
   */
  Distance TargetBody::sigmaRadiusA() const {
    return m_sigmaRadii[0];
  }

  /**
   * returns "b" radius
   *
   * @return Distance value of body radius "b"
   */
  Distance TargetBody::radiusB() const {
    return m_radii[1];
  }


  /**
   * returns "b" radius sigma
   *
   * @return Distance value of body radius "b" sigma
   */
  Distance TargetBody::sigmaRadiusB() const {
    return m_sigmaRadii[1];
  }


  /**
   * returns "c" radius
   *
   * @return Distance value of body radius "c"
   */
  Distance TargetBody::radiusC() const {
    return m_radii[2];
  }


  /**
   * returns "c" radius sigma
   *
   * @return Distance value of body radius "c" sigma
   */
  Distance TargetBody::sigmaRadiusC() const {
    return m_sigmaRadii[2];
  }


  /**
   * returns mean radius
   *
   * @return Distance value of body mean radius
   */
  Distance TargetBody::meanRadius() const {
    Distance meanRadius = m_radii[0] + m_radii[1] + m_radii[2];

    meanRadius = meanRadius/3.0;

    return meanRadius;
  }


  /**
   * returns mean radius sigma
   *
   * @return Distance value of body mean radius
   */
  Distance TargetBody::sigmaMeanRadius() const {
    Distance sigmaMeanRadius = m_sigmaRadii[0] + m_sigmaRadii[1] + m_sigmaRadii[2];

    sigmaMeanRadius = sigmaMeanRadius/3.0;

    return sigmaMeanRadius;
  }


  /**
   * Output format:
   *
   *
   * <image id="..." fileName="...">
   *   ...
   * </image>
   *
   * (fileName attribute is just the base name)
   */
//  void TargetBody::save(QXmlStreamWriter &stream, const Project *project,
//                            FileName newProjectRoot) const {

//    stream.writeStartElement("TargetBody");
//    // save ID, cnet file name, and run time to stream
//    stream.writeStartElement("generalAttributes");
//    stream.writeTextElement("id", m_id->toString());
//    stream.writeTextElement("runTime", runTime());
//    stream.writeTextElement("fileName", m_controlNetworkFileName->expanded());
//    stream.writeEndElement(); // end general attributes

//    // save settings to stream
//    m_settings->save(stream, project);

//    // save statistics to stream
//    m_statisticsResults->save(stream, project);

//    // save image lists to stream
//    if ( !m_images->isEmpty() ) {
//      stream.writeStartElement("imageLists");

//      for (int i = 0; i < m_images->count(); i++) {
//        m_images->at(i)->save(stream, project, "");
//      }

//      stream.writeEndElement();
//    }
//    stream.writeEndElement(); //end TargetBody
//  }



//  void TargetBody::save(QXmlStreamWriter &stream, const Project *project) const {

//    stream.writeStartElement("TargetBody");

//    // save ID, attributes, and run time to stream
//    stream.writeStartElement("generalAttributes");
//    stream.writeTextElement("id", m_id->toString());
//    stream.writeTextElement("runTime", runTime());
//    stream.writeEndElement(); // end general attributes

//    stream.writeEndElement(); //end TargetBody
//  }



  /**
   * Create an XML Handler (reader) that can populate the BundleSettings class data. See
   *   BundleSettings::save() for the expected format.
   *
   * @param bundleSettings The image we're going to be initializing
   * @param imageFolder The folder that contains the Cube
   */
//  TargetBody::XmlHandler::XmlHandler(TargetBody *TargetBody, Project *project) {
//    m_xmlHandlerTargetBody = TargetBody;
//    m_xmlHandlerProject = NULL;
//    m_xmlHandlerProject = project;
//    m_xmlHandlerCharacters = "";
//  }



//  TargetBody::XmlHandler::~XmlHandler() {
    // TargetBody passed in is "this" delete+null will cause problems,no?
//    delete m_xmlHandlerTargetBody;
//    m_xmlHandlerTargetBody = NULL;

    // we do not delete this pointer since it was set to a passed in pointer in constructor and we
    // don't own it... is that right???
//    delete m_xmlHandlerProject;
//    m_xmlHandlerProject = NULL;
//  }



  /**
   * Handle an XML start element. This expects <image/> and <displayProperties/> elements.
   *
   * @return If we should continue reading the XML (usually true).
   */
//  bool TargetBody::XmlHandler::startElement(const QString &namespaceURI, const QString &localName,
//                                       const QString &qName, const QXmlAttributes &atts) {
//    m_xmlHandlerCharacters = "";
//
//    if (XmlStackedHandler::startElement(namespaceURI, localName, qName, atts)) {
//
//      if (localName == "targetBody") {
//        m_xmlHandlerTargetBody =
//            BundleSettingsQsp(new TargetBody(m_xmlHandlerProject, reader()));
//      }
//      else if (localName == "bundleResults") {
//        delete m_xmlHandlerBundleResults;
//        m_xmlHandlerBundleResults = NULL;
//        m_xmlHandlerBundleResults = new BundleResults(m_xmlHandlerProject, reader()); //TODO: need to add constructor for this???
//      }
//      else if (localName == "imageList") {
//        m_xmlHandlerImages->append(new ImageList(m_xmlHandlerProject, reader()));
//      }
//    }
//    return true;
//  }



//  bool TargetBody::XmlHandler::characters(const QString &ch) {
//    m_xmlHandlerCharacters += ch;
//    return XmlStackedHandler::characters(ch);
//  }



//  bool TargetBody::XmlHandler::endElement(const QString &namespaceURI, const QString &localName,
//                                             const QString &qName) {
//    if (localName == "id") {
//      m_xmlHandlerTargetBody->m_id = NULL;
//      m_xmlHandlerTargetBody->m_id = new QUuid(m_xmlHandlerCharacters);
//    }
//    else if (localName == "runTime") {
//      m_xmlHandlerTargetBody->m_runTime = m_xmlHandlerCharacters;
//    }
//    else if (localName == "fileName") {
//      m_xmlHandlerTargetBody->m_controlNetworkFileName = NULL;
//      m_xmlHandlerTargetBody->m_controlNetworkFileName = new FileName(m_xmlHandlerCharacters);
//    }
//    else if (localName == "bundleSettings") {
//      m_xmlHandlerTargetBody->m_settings =
//          BundleSettingsQsp(new BundleSettings(*m_xmlHandlerBundleSettings));
//    }
//    else if (localName == "bundleResults") {
//      m_xmlHandlerTargetBody->m_statisticsResults = new BundleResults(*m_xmlHandlerBundleResults);
//    }
//    if (localName == "imageLists") {
//      for (int i = 0; i < m_xmlHandlerImages->size(); i++) {
//        m_xmlHandlerTargetBody->m_images->append(m_xmlHandlerImages->at(i));
//      }
//      m_xmlHandlerImages->clear();
//    }
//    m_xmlHandlerCharacters = "";
//    return XmlStackedHandler::endElement(namespaceURI, localName, qName);
//  }



  /**
   * Get a unique, identifying string associated with this TargetBody object.
   *
   * @return A unique ID for this TargetBody object
   */
  QString TargetBody::id() const {
    return m_id->toString().remove(QRegExp("[{}]"));
  }



//  QDataStream &TargetBody::write(QDataStream &stream) const {
//    stream << m_id->toString()
//           << m_runTime;

//    return stream;
//  }



//  QDataStream &TargetBody::read(QDataStream &stream) {

//    QString id;
//    stream >> id;
//    delete m_id;
//    m_id = NULL;
//    m_id = new QUuid(id);

//    stream >> m_runTime;

//    return stream;
//  }



//  QDataStream &operator<<(QDataStream &stream, const TargetBody &TargetBody) {
//    return TargetBody.write(stream);
//  }



//  QDataStream &operator>>(QDataStream &stream, TargetBody &TargetBody) {
//    return TargetBody.read(stream);
//  }



//  void TargetBody::savehdf5(FileName outputfilename) const {
//    const H5std_string  hdfFileName(outputfilename.expanded().toStdString()); //Is this the right way to have a dynamic file name?  What about PATH?
    
    
//    // Try block to detect exceptions raised by any of the calls inside it
//    try {
//      /*
//       * Turn off the auto-printing when failure occurs so that we can
//       * handle the errors appropriately
//       */
//      H5::Exception::dontPrint();
//      /*
//       * Create a new file using H5F_ACC_TRUNC access,
//       * default file creation properties, and default file
//       * access properties.
//       */
//      H5::H5File hdfFile = H5::H5File( hdfFileName, H5F_ACC_EXCL );
//      hid_t fileId = hdfFile.getId();

//      QString objectName = "/TargetBody";
//      H5LTset_attribute_string(fileId, objectName.toAscii(), "runTime", m_runTime.toAscii());
//      H5LTset_attribute_string(fileId, objectName.toAscii(), "controlNetworkFileName",
//                               m_controlNetworkFileName->expanded().toAscii());

//      //??? H5::Group settingsGroup = H5::Group(hdfFile.createGroup("/TargetBody/BundleSettings"));//???
//      //???H5::Group settingsGroup = hdfFile.createGroup("/TargetBody/BundleSettings");
//      QString groupName = objectName + "/BundleSettings";
//      hid_t groupId = H5Gcreate(fileId, groupName.toAscii(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
//      m_settings->savehdf5(groupId, groupName.toAscii());
//      groupName = objectName + "/BundleResults";
//      H5::Group resultsGroup  = H5::Group(hdfFile.createGroup(groupName.toAscii()));
//      m_statisticsResults->savehdf5(fileId, resultsGroup);
      
//    }
//    catch (H5::FileIException error) {
//      QString msg = QString(error.getCDetailMsg());
//      IException hpfError(IException::Unknown, msg, _FILEINFO_);
//      msg = "Unable to save TargetBody to hpf5 file. "
//            "H5 exception handler has detected a file error.";
//      throw IException(hpfError, IException::Unknown, msg, _FILEINFO_);
//    }
//  }
}