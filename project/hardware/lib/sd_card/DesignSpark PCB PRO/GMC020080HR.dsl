SamacSys ECAD Model
2446270/1167680/2.50/15/4/Connector

DESIGNSPARK_INTERMEDIATE_ASCII

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r150_100"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 1.000) (shapeHeight 1.500))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(padStyleDef "c80_h160"
		(holeDiam 1.6)
		(padShape (layerNumRef 1) (padShapeType Ellipse)  (shapeWidth 0.800) (shapeHeight 0.800))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0.800) (shapeHeight 0.800))
	)
	(padStyleDef "c55_h110"
		(holeDiam 1.1)
		(padShape (layerNumRef 1) (padShapeType Ellipse)  (shapeWidth 0.550) (shapeHeight 0.550))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0.550) (shapeHeight 0.550))
	)
	(padStyleDef "r280_150"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 1.500) (shapeHeight 2.800))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(padStyleDef "r150_70"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 0.700) (shapeHeight 1.500))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(textStyleDef "Default"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 50 mils)
			(strokeWidth 5 mils)
		)
	)
	(patternDef "GMC020080HR" (originalName "GMC020080HR")
		(multiLayer
			(pad (padNum 1) (padStyleRef r150_100) (pt -6.870, -9.000) (rotation 0))
			(pad (padNum 2) (padStyleRef r150_100) (pt -4.370, -9.000) (rotation 0))
			(pad (padNum 3) (padStyleRef r150_100) (pt -1.870, -9.000) (rotation 0))
			(pad (padNum 4) (padStyleRef r150_100) (pt 0.630, -9.000) (rotation 0))
			(pad (padNum 5) (padStyleRef r150_100) (pt 3.130, -9.000) (rotation 0))
			(pad (padNum 6) (padStyleRef r150_100) (pt 5.630, -9.000) (rotation 0))
			(pad (padNum 7) (padStyleRef r150_100) (pt 8.130, -9.000) (rotation 0))
			(pad (padNum 8) (padStyleRef r150_100) (pt 9.750, -9.000) (rotation 0))
			(pad (padNum 9) (padStyleRef r150_100) (pt -9.370, -9.000) (rotation 0))
			(pad (padNum 10) (padStyleRef c80_h160) (pt -9.500, -6.200) (rotation 90))
			(pad (padNum 11) (padStyleRef c55_h110) (pt 11.500, -6.200) (rotation 90))
			(pad (padNum 12) (padStyleRef r280_150) (pt -13.650, 7.300) (rotation 0))
			(pad (padNum 13) (padStyleRef r280_150) (pt -11.450, -7.200) (rotation 0))
			(pad (padNum 14) (padStyleRef r150_70) (pt 10.970, -9.000) (rotation 0))
			(pad (padNum 15) (padStyleRef r150_70) (pt 12.180, -9.000) (rotation 0))
			(pad (padNum 16) (padStyleRef r280_150) (pt 13.650, -7.200) (rotation 0))
			(pad (padNum 17) (padStyleRef r280_150) (pt 13.650, 7.300) (rotation 0))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt -0.000, -0.725) (textStyleRef "Default") (isVisible True))
		)
		(layerContents (layerNumRef 28)
			(line (pt -12.1 -8.3) (pt 12.1 -8.3) (width 0.2))
		)
		(layerContents (layerNumRef 28)
			(line (pt 12.1 -8.3) (pt 12.1 8.3) (width 0.2))
		)
		(layerContents (layerNumRef 28)
			(line (pt 12.1 8.3) (pt -12.1 8.3) (width 0.2))
		)
		(layerContents (layerNumRef 28)
			(line (pt -12.1 8.3) (pt -12.1 -8.3) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -12.1 8.3) (pt -12.1 8.3) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -12.1 8.3) (pt -12.1 -5) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -12.1 -5) (pt -12.1 -5) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -12.1 -5) (pt -12.1 8.3) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 12.1 -5) (pt 12.1 -5) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 12.1 -5) (pt 12.1 8.3) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 12.1 8.3) (pt 12.1 8.3) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 12.1 8.3) (pt 12.1 -5) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 12.1 8.3) (pt -12.1 8.3) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -12.1 8.3) (pt -12.1 8.3) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -12.1 8.3) (pt 12.1 8.3) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 12.1 8.3) (pt 12.1 8.3) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt -15.4 9.7) (pt 15.4 9.7) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt 15.4 9.7) (pt 15.4 -11.15) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt 15.4 -11.15) (pt -15.4 -11.15) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt -15.4 -11.15) (pt -15.4 9.7) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -7 -10.1) (pt -7 -10.1) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -6.95, -10.1) (radius 0.05) (startAngle 180.0) (sweepAngle 180.0) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -6.9 -10.1) (pt -6.9 -10.1) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -6.95, -10.1) (radius 0.05) (startAngle .0) (sweepAngle 180.0) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -7 -10.1) (pt -7 -10.1) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -6.95, -10.1) (radius 0.05) (startAngle 180.0) (sweepAngle 180.0) (width 0.2))
		)
	)
	(symbolDef "GMC020080HR" (originalName "GMC020080HR")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -25 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 2) (pt 0 mils -100 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -125 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 3) (pt 0 mils -200 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -225 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 4) (pt 0 mils -300 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -325 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 5) (pt 0 mils -400 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -425 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 6) (pt 0 mils -500 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -525 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 7) (pt 0 mils -600 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -625 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 8) (pt 0 mils -700 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -725 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 9) (pt 1400 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -25 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 10) (pt 1400 mils -100 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -125 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 11) (pt 1400 mils -200 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -225 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 12) (pt 1400 mils -300 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -325 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 13) (pt 1400 mils -400 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -425 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 14) (pt 1400 mils -500 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -525 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 15) (pt 1400 mils -600 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -625 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(line (pt 200 mils 100 mils) (pt 1200 mils 100 mils) (width 6 mils))
		(line (pt 1200 mils 100 mils) (pt 1200 mils -800 mils) (width 6 mils))
		(line (pt 1200 mils -800 mils) (pt 200 mils -800 mils) (width 6 mils))
		(line (pt 200 mils -800 mils) (pt 200 mils 100 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 1250 mils 300 mils) (justify Left) (isVisible True) (textStyleRef "Default"))

	)
	(compDef "GMC020080HR" (originalName "GMC020080HR") (compHeader (numPins 15) (numParts 1) (refDesPrefix J)
		)
		(compPin "1" (pinName "CD/DAT3") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "2" (pinName "CMD") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "3" (pinName "VSS1") (partNum 1) (symPinNum 3) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "4" (pinName "VDD") (partNum 1) (symPinNum 4) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "5" (pinName "CLK") (partNum 1) (symPinNum 5) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "6" (pinName "VSS2") (partNum 1) (symPinNum 6) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "7" (pinName "DAT0") (partNum 1) (symPinNum 7) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "8" (pinName "DAT1") (partNum 1) (symPinNum 8) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "9" (pinName "DAT2") (partNum 1) (symPinNum 9) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "MP1" (pinName "GND_1") (partNum 1) (symPinNum 10) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "MP2" (pinName "GND_2") (partNum 1) (symPinNum 11) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "MP3" (pinName "CD PIN") (partNum 1) (symPinNum 12) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "MP4" (pinName "WP PIN") (partNum 1) (symPinNum 13) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "MP5" (pinName "GND_3") (partNum 1) (symPinNum 14) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "MP6" (pinName "GND_4") (partNum 1) (symPinNum 15) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "GMC020080HR"))
		(attachedPattern (patternNum 1) (patternName "GMC020080HR")
			(numPads 15)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
				(padNum 3) (compPinRef "3")
				(padNum 4) (compPinRef "4")
				(padNum 5) (compPinRef "5")
				(padNum 6) (compPinRef "6")
				(padNum 7) (compPinRef "7")
				(padNum 8) (compPinRef "8")
				(padNum 9) (compPinRef "9")
				(padNum 10) (compPinRef "MP1")
				(padNum 11) (compPinRef "MP2")
				(padNum 12) (compPinRef "MP3")
				(padNum 13) (compPinRef "MP4")
				(padNum 14) (compPinRef "MP5")
				(padNum 15) (compPinRef "MP6")
			)
		)
		(attr "Mouser Part Number" "523-GMC020080HR")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/Amphenol-Commercial-Products/GMC020080HR?qs=W0yvOO0ixfGUck2%252BUqFamw%3D%3D")
		(attr "Manufacturer_Name" "Amphenol")
		(attr "Manufacturer_Part_Number" "GMC020080HR")
		(attr "Description" "Memory Card Connectors 2 In 1 Connector Sd+mmc")
		(attr "Datasheet Link" "https://componentsearchengine.com/Datasheets/1/GMC020080HR.pdf")
		(attr "Height" "2.87 mm")
	)

)
