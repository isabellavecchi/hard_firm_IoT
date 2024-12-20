PULSONIX_LIBRARY_ASCII "SamacSys ECAD Model"
//725511/1167680/2.50/9/4/MOSFET N-Channel

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r63_50"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 0.500) (shapeHeight 0.630))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(padStyleDef "r50_41"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 0.410) (shapeHeight 0.500))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(padStyleDef "r245_190"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 1.900) (shapeHeight 2.450))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(textStyleDef "Normal"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 1.27)
			(strokeWidth 0.127)
		)
	)
	(patternDef "CSD17575Q3T" (originalName "CSD17575Q3T")
		(multiLayer
			(pad (padNum 1) (padStyleRef r63_50) (pt -0.975, -1.435) (rotation 0))
			(pad (padNum 2) (padStyleRef r63_50) (pt -0.325, -1.435) (rotation 0))
			(pad (padNum 3) (padStyleRef r63_50) (pt 0.325, -1.435) (rotation 0))
			(pad (padNum 4) (padStyleRef r63_50) (pt 0.975, -1.435) (rotation 0))
			(pad (padNum 5) (padStyleRef r50_41) (pt 0.975, 1.545) (rotation 90))
			(pad (padNum 6) (padStyleRef r50_41) (pt 0.325, 1.545) (rotation 90))
			(pad (padNum 7) (padStyleRef r50_41) (pt -0.325, 1.545) (rotation 90))
			(pad (padNum 8) (padStyleRef r50_41) (pt -0.975, 1.545) (rotation 90))
			(pad (padNum 9) (padStyleRef r245_190) (pt 0.000, 0.390) (rotation 90))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0.000, 0.000) (textStyleRef "Normal") (isVisible True))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.65 -1.65) (pt 1.65 -1.65) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.65 -1.65) (pt 1.65 1.65) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.65 1.65) (pt -1.65 1.65) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.65 1.65) (pt -1.65 -1.65) (width 0.025))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -2.65 2.75) (pt 2.65 2.75) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 2.65 2.75) (pt 2.65 -2.75) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 2.65 -2.75) (pt -2.65 -2.75) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -2.65 -2.75) (pt -2.65 2.75) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1 -2.2) (pt -1 -2.2) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -1, -2.3) (radius 0.1) (startAngle 90.0) (sweepAngle 180.0) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1 -2.4) (pt -1 -2.4) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -1, -2.3) (radius 0.1) (startAngle 270) (sweepAngle 180.0) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1 -2.2) (pt -1 -2.2) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -1, -2.3) (radius 0.1) (startAngle 90.0) (sweepAngle 180.0) (width 0.2))
		)
	)
	(symbolDef "CSD17575Q3T" (originalName "CSD17575Q3T")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -25 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 2) (pt 0 mils -100 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -125 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 3) (pt 0 mils -200 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -225 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 4) (pt 0 mils -300 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -325 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 5) (pt 0 mils -400 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -425 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 6) (pt 1000 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 770 mils -25 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 7) (pt 1000 mils -100 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 770 mils -125 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 8) (pt 1000 mils -200 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 770 mils -225 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 9) (pt 1000 mils -300 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 770 mils -325 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(line (pt 200 mils 100 mils) (pt 800 mils 100 mils) (width 6 mils))
		(line (pt 800 mils 100 mils) (pt 800 mils -500 mils) (width 6 mils))
		(line (pt 800 mils -500 mils) (pt 200 mils -500 mils) (width 6 mils))
		(line (pt 200 mils -500 mils) (pt 200 mils 100 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 850 mils 300 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))
		(attr "Type" "Type" (pt 850 mils 200 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))

	)
	(compDef "CSD17575Q3T" (originalName "CSD17575Q3T") (compHeader (numPins 9) (numParts 1) (refDesPrefix Q)
		)
		(compPin "1" (pinName "S_1") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "2" (pinName "S_2") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "3" (pinName "S_3") (partNum 1) (symPinNum 3) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "4" (pinName "G") (partNum 1) (symPinNum 4) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "5" (pinName "D_1") (partNum 1) (symPinNum 5) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "6" (pinName "D_2") (partNum 1) (symPinNum 6) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "7" (pinName "D_3") (partNum 1) (symPinNum 7) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "8" (pinName "D_4") (partNum 1) (symPinNum 8) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "9" (pinName "D_5") (partNum 1) (symPinNum 9) (gateEq 0) (pinEq 0) (pinType Unknown))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "CSD17575Q3T"))
		(attachedPattern (patternNum 1) (patternName "CSD17575Q3T")
			(numPads 9)
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
			)
		)
		(attr "Mouser Part Number" "595-CSD17575Q3T")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/Texas-Instruments/CSD17575Q3T?qs=1MXlzafEN3%252BV4dVr3K7EZA%3D%3D")
		(attr "Manufacturer_Name" "Texas Instruments")
		(attr "Manufacturer_Part_Number" "CSD17575Q3T")
		(attr "Description" "30V, N ch NexFET MOSFET, single SON3x3, 3.2mOhm")
		(attr "<Hyperlink>" "http://www.ti.com/lit/gpn/CSD17575Q3")
		(attr "<Component Height>" "1.1")
		(attr "<STEP Filename>" "CSD17575Q3T.stp")
		(attr "<STEP Offsets>" "X=0;Y=0;Z=0")
		(attr "<STEP Rotation>" "X=0;Y=0;Z=0")
	)

)
